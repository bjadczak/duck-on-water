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

class Water
{
    std::unique_ptr<Mesh<PosNorTexVertex>> waterMesh;

    std::unique_ptr<Texture> waterHeightMap;

    Material material{{0,0,1,1},0.0,0};

    void generateWaterPlane(float width, float height, float depth);

    void sendDataToTexture(float** data);

    float N = 256.0f;
    float h = 2 / (N - 1);
    float c = 1.0f;
    float t = 1 / N;
    float A = (c * c * t * t) / (h * h);
    float B = 2 - 4 * A;
    float** tempTable;
    float** heightTable;
    float** heightTable2;
    float*** data4Dim;
    bool swapTables = false;

public:
    Water();
    void render(Shader &shader);
    void update();
};


#endif //DUCKONWATER_WATER_H
