//
// Created by Bartek Jadczak on 31/05/2024.
//

#include "Duck.h"

Duck::Duck (Model<PosNorTexVertex> &sourceModel)
{
    model = std::make_unique<Mesh<PosNorTexVertex>>(sourceModel.vertices, sourceModel.triagleIndices);
    texture = std::make_unique<Texture>("../res/textures/ducktex.jpg");
}

void Duck::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);

    shader.setUniform("model",
                      glm::scale(
                              glm::translate(
                                      glm::mat4(1),
                                      glm::vec3(0.0f, 0.2f, 0.0f)
                                        ),

    glm::vec3(0.001f, 0.001f, 0.001f)
                                    )
                     );
    texture->bind(0);
    model->render();
}
