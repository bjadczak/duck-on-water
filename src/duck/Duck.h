//
// Created by Bartek Jadczak on 31/05/2024.
//

#ifndef DUCKONWATER_DUCK_H
#define DUCKONWATER_DUCK_H

#include "../Shader.h"
#include "../mesh/Mesh.h"
#include "../mesh/PositionNormalVertex.h"
#include "../importer/Model.h"
#include "../material/Material.h"
#include "../mesh/PosNorTexVertex.h"
#include "../texture/Texture.h"

class Duck
{
    std::unique_ptr<Mesh<PosNorTexVertex>> model;
    std::unique_ptr<Texture> texture;
    Material material{{1,1,1,1},0.0,0};

public:

    explicit Duck(Model<PosNorTexVertex>& model);

    void render(Shader &shader);
};


#endif //DUCKONWATER_DUCK_H
