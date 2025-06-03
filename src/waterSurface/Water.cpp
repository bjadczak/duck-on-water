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

Water:: Water()
{
    generateWaterPlane(2, 0.2f, 2);

    for (int i = 0; i < NORMAL_MAP_H; i++) {
        for (int j = 0; j < NORMAL_MAP_W; j++) {
            heightTable[i * NORMAL_MAP_W + j] = heightTable2[i * NORMAL_MAP_W + j] = tempTable[i * NORMAL_MAP_W + j] = 0;
        }
    }

    // Allocate memory for the 3D array of pointers
    //data4Dim = new float[NORMAL_MAP_H * NORMAL_MAP_W * 4];

    data4Dim[(NORMAL_MAP_W * 0 + 0) * COLOR_MAP_COUNT + 0] = 0;
    data4Dim[(NORMAL_MAP_W * 0 + 0) * COLOR_MAP_COUNT + 1] = 0;
    data4Dim[(NORMAL_MAP_W * 0 + 0) * COLOR_MAP_COUNT + 2] = 0;
    data4Dim[(NORMAL_MAP_W * 0 + 0) * COLOR_MAP_COUNT + 3] = 1;

    data4Dim[(NORMAL_MAP_W * 255 + 255) * COLOR_MAP_COUNT + 0] = 0;
    data4Dim[(NORMAL_MAP_W * 255 + 255) * COLOR_MAP_COUNT + 1] = 0;
    data4Dim[(NORMAL_MAP_W * 255 + 255) * COLOR_MAP_COUNT + 2] = 0;
    data4Dim[(NORMAL_MAP_W * 255 + 255) * COLOR_MAP_COUNT + 3] = 1;

    waterHeightMap = std::make_unique<Texture>(data4Dim, NORMAL_MAP_H, NORMAL_MAP_W);

    update();

}

void Water::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);
    shader.setUniform("model", glm::mat4(1));
    shader.setUniform("waterLevel", waterLevel);
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
                tempTable[i * NORMAL_MAP_W + j] =
                        d * (A * (heightTable[(i + 1) * NORMAL_MAP_W + j] + heightTable[(i - 1) * NORMAL_MAP_W + j] +
                                  heightTable[i * NORMAL_MAP_W + (j - 1)] + heightTable[i * NORMAL_MAP_W + (j + 1)]) +
                             B * heightTable[i * NORMAL_MAP_W + j] - heightTable2[i * NORMAL_MAP_W + j]);
            } else {
                tempTable[i * NORMAL_MAP_W + j] =
                        d * (A * (heightTable2[(i + 1) * NORMAL_MAP_W + j] + heightTable2[(i - 1) * NORMAL_MAP_W + j] +
                                  heightTable2[i * NORMAL_MAP_W + (j - 1)] + heightTable2[i * NORMAL_MAP_W + (j + 1)]) +
                             B * heightTable2[i * NORMAL_MAP_W + j] - heightTable[i * NORMAL_MAP_W + j]);
            }
        }
    }

    if (std::rand() % 10 == 0) {
        int i = std::rand() % 256;
        int j = std::rand() % 256;
        tempTable[i * NORMAL_MAP_W + j] = 0.25f;
    }
    tempTable[(int)((duckPos.z + 1)/ 2 * 256) * NORMAL_MAP_W + (int)((duckPos.x + 1) / 2 * 256)] = 0.25f;
    swapTables = !swapTables;

    sendDataToTexture(tempTable);

    for (int i = 0; i < NORMAL_MAP_H; i++)
    {
        for (int j = 0; j < NORMAL_MAP_W; j++)
        {
            if (swapTables)
            {
                heightTable[i * NORMAL_MAP_W + j] = tempTable[i * NORMAL_MAP_W + j];
            }
            else
            {
                heightTable2[i * NORMAL_MAP_W + j] = tempTable[i * NORMAL_MAP_W + j];
            }
        }
    }
}

void Water::sendDataToTexture (float *data)
{
    static const float eps = 1.0f / 255.0f;

    for(int i = 1; i < NORMAL_MAP_H - 1; i++)
    {
        for(int j = 1; j < NORMAL_MAP_W - 1; j++)
        {
            float dx = data[(i - 1) * NORMAL_MAP_W + j] - data[(i + 1) * NORMAL_MAP_W + j];
            float dy = data[i * NORMAL_MAP_W + (j - 1)] - data[i * NORMAL_MAP_W + (j + 1)];

            glm::vec3 v1 = glm::vec3 (2.0 / 256.0, dx, 0.0);
            glm::vec3 v2 = glm::vec3(0.0, dy, 2.0 / 256.0);
            glm::vec3 normal = glm::normalize(glm::cross(v2, v1));

            data4Dim[(NORMAL_MAP_W * i + j) * COLOR_MAP_COUNT + 0] = normal.x;
            data4Dim[(NORMAL_MAP_W * i + j) * COLOR_MAP_COUNT + 1] = normal.y;
            data4Dim[(NORMAL_MAP_W * i + j) * COLOR_MAP_COUNT + 2] = normal.z;
            data4Dim[(NORMAL_MAP_W * i + j) * COLOR_MAP_COUNT + 3] = 1;

        }
    }

    waterHeightMap->updateTexture(data4Dim, NORMAL_MAP_H, NORMAL_MAP_W);

}

void Water::updateDuckPos (glm::vec3 duckPos)
{
    this->duckPos = duckPos;
}
