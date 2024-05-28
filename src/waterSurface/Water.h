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

class Water
{
    std::unique_ptr<Mesh<PositionNormalVertex>> waterMesh;

    Material material{{0,0,1,1},0.0,0};

    void generateWaterPlane(float width, float height, float depth);
public:
    Water();
    void render(Shader &shader);
};


#endif //DUCKONWATER_WATER_H
