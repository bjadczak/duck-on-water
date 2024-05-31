//
// Created by faliszewskii on 27.04.24.
//

#include <fstream>
#include <stdexcept>
#include <sstream>
#include "Importer.h"

Model<PosNorTexVertex> Importer::loadModel(const std::string &path) {
    Model<PosNorTexVertex> model;

    auto rawModel = loadRawModel(path);
    for(int i=0; i < rawModel.vertices.size(); i++)
    {
        model.vertices.push_back({rawModel.vertices[i], rawModel.normals[i], rawModel.texCords[i]});
    }

    model.triagleIndices.reserve(3 * rawModel.triangles.size());
    for(auto &triangle : rawModel.triangles) {
        model.triagleIndices.push_back(triangle[0]);
        model.triagleIndices.push_back(triangle[1]);
        model.triagleIndices.push_back(triangle[2]);
    }

    return model;
}

RawModel Importer::loadRawModel(const std::string &path) {
    RawModel model;

    std::ifstream file(path);
    if (!file.is_open()) {
        throw(std::runtime_error("Error opening file."));
    }

    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
    }

    int caret = 0;

    // Unique vertices section starts with their count.
    int verticesCount = std::stoi(tokens[caret++]);
    // Load unique vertices
    model.vertices.reserve(verticesCount);
    model.normals.reserve(verticesCount);
    model.texCords.reserve(verticesCount);
    for(int i=0; i < verticesCount; i++) {
        glm::vec3 vertex(std::stof(tokens[caret+0]), std::stof(tokens[caret+1]), std::stof(tokens[caret+2]));
        glm::vec3 normal(std::stof(tokens[caret+3]), std::stof(tokens[caret+4]), std::stof(tokens[caret+5]));
        glm::vec2 tex(std::stof(tokens[caret+6]), std::stof(tokens[caret+7]));
        caret+=8;
        model.vertices.push_back(vertex);
        model.normals.push_back(normal);
        model.texCords.push_back(tex);
    }

    // Triangle section starts with their count.
    int triangleCount = std::stoi(tokens[caret++]);
    // Load indices from triangles
    model.triangles.reserve(triangleCount);
    for(int i=0; i < triangleCount; i++) {
        model.triangles.push_back({
            static_cast<unsigned int>(std::stoi(tokens[caret+0])),
            static_cast<unsigned int>(std::stoi(tokens[caret+1])),
            static_cast<unsigned int>(std::stoi(tokens[caret+2])),
            });
        caret+=3;
    }


    file.close();
    return model;
}
