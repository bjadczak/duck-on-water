//
// Created by Bartek Jadczak on 28/05/2024.
//

#ifndef DUCKONWATER_WATER_H
#define DUCKONWATER_WATER_H

#include "../Shader.h"
#include "../mesh/Mesh.h"
#include "../mesh/PositionNormalVertex.h"
#include "../importer/Model.h"
#include "../material/Material.h"
#include "../mesh/PosNorTexVertex.h"
#include "../texture/Texture.h"

#define NORMAL_MAP_H 256
#define NORMAL_MAP_W 256
#define COLOR_MAP_COUNT 4

class Water
{
    std::unique_ptr<Mesh<PosNorTexVertex>> waterMesh;

    std::unique_ptr<Texture> waterHeightMap;

    Material material{{0,0,1,1},0.0,0};

    void generateWaterPlane(float width, float height, float depth);

    void sendDataToTexture(float* data);

    float N = 256.0f;
    float h = 2 / (N - 1);
    float c = 1.0f;
    float t = 1 / N;
    float A = (c * c * t * t) / (h * h);
    float B = 2 - 4 * A;
    float tempTable[NORMAL_MAP_H * NORMAL_MAP_W];
    float heightTable[NORMAL_MAP_H * NORMAL_MAP_W];
    float heightTable2[NORMAL_MAP_H * NORMAL_MAP_W];
    float data4Dim[NORMAL_MAP_H * NORMAL_MAP_W * 4];
    bool swapTables = false;

    glm::vec3 duckPos = glm::vec3(0, 0.2f, 0);

public:
    Water();
    void render(Shader &shader);
    void update();
    void updateDuckPos(glm::vec3 duckPos);

    float waterLevel = 0.2f;
};


#endif //DUCKONWATER_WATER_H
