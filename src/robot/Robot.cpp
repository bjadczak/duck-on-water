//
// Created by faliszewskii on 18.04.24.
//

#include "Robot.h"

Robot::Robot(Model& standModel, std::vector<Model>& armModels) {
    standMesh = std::make_unique<Mesh<PositionNormalVertex>>(standModel.vertices, standModel.triagleIndices);
    armMeshes.reserve(armModels.size());
    for(auto &armModel : armModels) {
        armMeshes.emplace_back(armModel.vertices, armModel.triagleIndices);
    }

    standMeshVolume = std::make_unique<Mesh<PositionNormalVertex>>(standModel.vertices, standModel.triagleAdjacencyIndices, GL_TRIANGLES_ADJACENCY);
    armMeshesVolume.reserve(armModels.size());
    for(auto &armModel : armModels) {
        armMeshesVolume.emplace_back(armModel.vertices, armModel.triagleAdjacencyIndices, GL_TRIANGLES_ADJACENCY);
    }
}

void Robot::update(float timeS) {
    kinematics.update(timeS);
}

void Robot::render(Shader &shader) {
    material.setupMaterial(shader);
    shader.setUniform("model", kinematics.getModel(0));
    standMesh->render();
    for (int i = 0; i < armMeshes.size(); i++) {
        shader.setUniform("model", kinematics.getModel(i+1));
        armMeshes[i].render();
    }
}

void Robot::renderShadow(Shader &shader) {
    shader.setUniform("model", kinematics.getModel(0));
    standMeshVolume->render();
    for (int i = 0; i < armMeshes.size(); i++) {
        shader.setUniform("model", kinematics.getModel(i+1));
        armMeshesVolume[i].render();
    }
}
