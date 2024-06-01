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

#define NUM_OF_CONTROL_PT 4

class Duck
{
    std::unique_ptr<Mesh<PosNorTexVertex>> model;
    std::unique_ptr<Texture> texture;
    Material material{{1,1,1,1},0.0,0};

    glm::vec3 pos;
    glm::mat4 rotation;

    glm::vec2 ctrlPt[NUM_OF_CONTROL_PT];

    glm::vec2 deCasteljau4(float t, glm::vec2 coefficients[4]);
    glm::vec2 deCasteljau3(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2);
    void updatePosAnDir();

    float t = 0.0f;

    void getNewControlPoints(glm::vec2 firstPoint);

public:

    explicit Duck(Model<PosNorTexVertex>& model);

    void render(Shader &shader);

    glm::vec3 getPos() { return pos; }

    void updatedWaterLevel(float newWaterLevel) {pos.y = newWaterLevel;}
};


#endif //DUCKONWATER_DUCK_H
