//
// Created by faliszewskii on 27.04.24.
//

#ifndef PUMAROBOT_RAWMODEL_H
#define PUMAROBOT_RAWMODEL_H

#include <vector>
#include <array>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct RawModel {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCords;

    std::vector<std::array<unsigned int, 3>> triangles;
};

#endif //PUMAROBOT_RAWMODEL_H
