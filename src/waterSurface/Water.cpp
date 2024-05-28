//
// Created by Bartek Jadczak on 28/05/2024.
//

#include "Water.h"

void Water::generateWaterPlane (float width, float height, float depth)
{
    std::vector<PosNorTexVertex> waterVertices = {
            // Bottom face
            {glm::vec3(-width / 2, height,  depth / 2), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0, 1)},
            {glm::vec3( width / 2, height,  depth / 2), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1, 1)},
            {glm::vec3( width / 2, height, -depth / 2), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1, 0)},
            {glm::vec3(-width / 2, height, -depth / 2), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0, 0)},
            // Top face
            {glm::vec3(-width / 2, height,  depth / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0)},
            {glm::vec3( width / 2, height,  depth / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1, 0)},
            {glm::vec3( width / 2, height, -depth / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1, 1)},
            {glm::vec3(-width / 2, height, -depth / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 1)},
    };

    std::vector<unsigned int> waterIndices = {
            // Bottom face
            0, 1, 2,
            2, 3, 0,
            4, 6, 5,
            6, 4, 7,
    };

    waterMesh = std::make_unique<Mesh<PosNorTexVertex>>(waterVertices, waterIndices);
}

Water::Water ()
{
    generateWaterPlane(6, 2, 6);
    tempTable = new float*[NORMAL_MAP_H];
    heightTable = new float*[NORMAL_MAP_H];
    heightTable2 = new float*[NORMAL_MAP_H];
    for(int i = 0; i < NORMAL_MAP_H; i++)
    {
        tempTable[i] = new float[NORMAL_MAP_W];
        heightTable[i] = new float[NORMAL_MAP_W];
        heightTable2[i] = new float[NORMAL_MAP_W];
    }
    for (int i = 0; i < NORMAL_MAP_H; i++) {
        for (int j = 0; j < NORMAL_MAP_W; j++) {
            heightTable[i][j] = heightTable2[i][j] = tempTable[i][j] = 0;
        }
    }

    // Allocate memory for the 3D array of pointers
    data4Dim = new float**[NORMAL_MAP_H];
    for (int y = 0; y < NORMAL_MAP_H; ++y)
    {
        data4Dim[y] = new float*[NORMAL_MAP_W];
        for (int x = 0; x < NORMAL_MAP_W; ++x)
        {
            data4Dim[y][x] = new float[4]; // 4 channels for RGBA
        }
    }

    data4Dim[0][0][0] = 0;
    data4Dim[0][0][1] = 0;
    data4Dim[0][0][2] = 0;
    data4Dim[0][0][3] = 1;

    data4Dim[255][255][0] = 0;
    data4Dim[255][255][1] = 0;
    data4Dim[255][255][2] = 0;
    data4Dim[255][255][3] = 1;

    waterHeightMap = std::make_unique<Texture>(data4Dim, NORMAL_MAP_H, NORMAL_MAP_W);

    update();

}

void Water::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);
    shader.setUniform("model", glm::mat4(1));
    waterHeightMap->bind(0);
    waterMesh->render();
    update();
}

void Water::update ()
{
    for (int i = 1; i < NORMAL_MAP_H - 1; i++) {
        for (int j = 1; j < NORMAL_MAP_W - 1; j++) {
            float leftBound = i / 256.0f;
            float rightBound = (256.0f - i) / 256.0f;
            float upperBound = j / 256.0f;
            float lowerBound = (256.0f - j) / 256.0f;

            float minLen =
                    std::min({leftBound, rightBound, upperBound, lowerBound}) * 2.0f;
            float d = 0.95f * std::min(1.0f, minLen / 0.2f);

            if (swapTables) {
                tempTable[i][j] =
                        d * (A * (heightTable[i + 1][j] + heightTable[i - 1][j] +
                                  heightTable[i][j - 1] + heightTable[i][j + 1]) +
                             B * heightTable[i][j] - heightTable2[i][j]);
            } else {
                tempTable[i][j] =
                        d * (A * (heightTable2[i + 1][j] + heightTable2[i - 1][j] +
                                  heightTable2[i][j - 1] + heightTable2[i][j + 1]) +
                             B * heightTable2[i][j] - heightTable[i][j]);
            }
        }
    }

    if (std::rand() % 10 == 0) {
        int i = std::rand() % 256;
        int j = std::rand() % 256;
        tempTable[i][j] = 0.25f;
    }
    swapTables = !swapTables;

    sendDataToTexture(tempTable);

    for (int i = 0; i < NORMAL_MAP_H; i++)
    {
        for (int j = 0; j < NORMAL_MAP_W; j++)
        {
            if (swapTables)
            {
                heightTable[i][j] = tempTable[i][j];
            }
            else
            {
                heightTable2[i][j] = tempTable[i][j];
            }
        }
    }
}

void Water::sendDataToTexture (float **data)
{
    static const float eps = 2.0f / 255.0f;

    for(int i = 1; i < 255; i++)
    {
        for(int j = 1; j < 255; j++)
        {
            float dx = data[i - 1][j    ] - data[i + 1][j    ];
            float dy = data[i    ][j - 1] - data[i    ][j + 1];

            glm::vec3 normal = glm::normalize(glm::vec3(dx / (2 * eps), dy / (2 * eps), 1.0f));
            data4Dim[i][j][0] = normal.x;
            data4Dim[i][j][1] = normal.y;
            data4Dim[i][j][2] = normal.z;
            data4Dim[i][j][3] = 1;

        }
    }

    waterHeightMap->updateTexture(data4Dim, NORMAL_MAP_H, NORMAL_MAP_W);


}
