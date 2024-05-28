//
// Created by Bartek Jadczak on 28/05/2024.
//

#include "Water.h"

void Water::generateWaterPlane (float width, float height, float depth)
{
    std::vector<PositionNormalVertex> waterVertices = {
            // Bottom face
            {glm::vec3(-width / 2, height,  depth / 2), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3( width / 2, height,  depth / 2), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3( width / 2, height, -depth / 2), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3(-width / 2, height, -depth / 2), glm::vec3(0.0f, 1.0f, 0.0f)},
            // Top face
            {glm::vec3(-width / 2, height,  depth / 2), glm::vec3(0.0f, -1.0f, 0.0f)},
            {glm::vec3( width / 2, height,  depth / 2), glm::vec3(0.0f, -1.0f, 0.0f)},
            {glm::vec3( width / 2, height, -depth / 2), glm::vec3(0.0f, -1.0f, 0.0f)},
            {glm::vec3(-width / 2, height, -depth / 2), glm::vec3(0.0f, -1.0f, 0.0f)},
    };

    std::vector<unsigned int> waterIndices = {
            // Bottom face
            0, 1, 2,
            2, 3, 0,
            4, 6, 5,
            6, 4, 7,
    };

    waterMesh = std::make_unique<Mesh<PositionNormalVertex>>(waterVertices, waterIndices);
}

Water::Water ()
{
    generateWaterPlane(6, 2, 6);
}

void Water::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", false);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);
    shader.setUniform("model", glm::mat4(1));
    waterMesh->render();
}
