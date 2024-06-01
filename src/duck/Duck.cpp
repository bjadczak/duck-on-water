//
// Created by Bartek Jadczak on 31/05/2024.
//

#include "Duck.h"
#include <random>

Duck::Duck (Model<PosNorTexVertex> &sourceModel)
{
    model = std::make_unique<Mesh<PosNorTexVertex>>(sourceModel.vertices, sourceModel.triagleIndices);
    texture = std::make_unique<Texture>("../res/textures/ducktex.jpg");
    pos = glm::vec3(0.0f, 0.2f, 0.0f);
    rotation = glm::mat4(1);
    getNewControlPoints(glm::vec2(0));
}

void Duck::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);

    shader.setUniform("model",
                      glm::scale(
                              rotation,

    glm::vec3(0.001f, 0.001f, 0.001f)
                                    )
                     );
    texture->bind(0);
    model->render();

    updatePosAnDir();
}

void Duck::getNewControlPoints (glm::vec2 firstPoint)
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    static std::uniform_real_distribution<float> unif(-1.0f, 1.0f);
    ctrlPt[0] = firstPoint;
    ctrlPt[1].x = unif(generator); ctrlPt[1].y = unif(generator);
    ctrlPt[2].x = unif(generator); ctrlPt[2].y = unif(generator);
    ctrlPt[3].x = unif(generator); ctrlPt[3].y = unif(generator);


    if(rand() % 2 == 0)
    {
        if(rand() % 2 == 0) ctrlPt[3].x =  1.0f;
        else                ctrlPt[3].x = -1.0f;
    }
    else
    {
        if(rand() % 2 == 0) ctrlPt[3].y =  1.0f;
        else                ctrlPt[3].y = -1.0f;
    }
}
glm::vec2 Duck::deCasteljau4(float t, glm::vec2 coefficientsIn[4])
{
    int n = 4;
    glm::vec2 coefficients[4] = {
            coefficientsIn[0],
            coefficientsIn[1],
            coefficientsIn[2],
            coefficientsIn[3],
            };
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < (n - i); j++) {
            coefficients[j] = coefficients[j] * (1 - t) + coefficients[j + 1] * t;
        }
    }
    return coefficients[0];
}
glm::vec2 Duck::deCasteljau3(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2)
{
    // Bernstein polynomials
    float b0 = (1 - t) * (1 - t);
    float b1 = 2.0 * t * (1 - t);
    float b2 = t * t;

    // Cubic Bezier interpolation
    return p0 * b0 + p1 * b1 + p2 * b2;
}
void Duck::updatePosAnDir()
{
    t += 0.005f;
    if(t >= 1.0f)
    {
        t = 0.0f;
        getNewControlPoints(ctrlPt[3]);
    }

    glm::vec2 calcPos = deCasteljau4(t, ctrlPt);
    glm::vec2 p0 = (- ctrlPt[0] + ctrlPt[1]) / 2.0f;
    glm::vec2 p1 = (- ctrlPt[1] + ctrlPt[2]) / 2.0f;
    glm::vec2 p2 = (- ctrlPt[2] + ctrlPt[3]) / 2.0f;

    pos.x = calcPos.x;
    pos.z = calcPos.y;

    glm::vec2 calcDir = glm::normalize(deCasteljau3(t, p0, p1, p2));


    float angle = atan2(calcDir.y, -calcDir.x);
    rotation = glm::rotate(glm::translate(glm::mat4(1), pos), angle, glm::vec3(0, 1, 0));


}
