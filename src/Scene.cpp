//
// Created by faliszewskii on 18.04.24.
//

#include "Scene.h"


Scene::Scene(AppContext &appContext) :
        whiteShader("../res/shaders/basic/position.vert", "../res/shaders/basic/white.frag"),
        phongShader("../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag"),
        skyboxShader("../res/shaders/skybox/skybox.vert","../res/shaders/skybox/skybox.frag"),
        pointShader("../res/shaders/point/point.vert","../res/shaders/point/point.frag"),
        appContext(appContext)
    {}

void Scene::update() {
    appContext.light->updateColor(glm::vec4(appContext.pointLight.color, appContext.pointLight.strength));
    appContext.light->updatePosition(appContext.pointLight.position);
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    drawSkybox();
    setupPhong(appContext.pointLight);

    drawScene();

    drawPointLight(*appContext.light);

    appContext.frameBufferManager->unbind();
}

void Scene::drawPointLight(Point &point) {
    pointShader.use();
    pointShader.setUniform("view", appContext.camera->getViewMatrix());
    pointShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    point.render(pointShader);
}

void Scene::drawSkybox() {
    skyboxShader.use();
    skyboxShader.setUniform("view", appContext.camera->getNoTranslationViewMatrix());
    skyboxShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    skyboxShader.setUniform("skybox", 0);
    appContext.skybox->render();
}


void Scene::setupPhong(PointLight &pointLight) {
    phongShader.use();
    phongShader.setUniform("view", appContext.camera->getViewMatrix());
    phongShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    phongShader.setUniform("viewPos", appContext.camera->getViewPosition());
    phongShader.setUniform("isMirror", false);
    pointLight.setupPointLight(phongShader);
}

void Scene::drawScene() {
    appContext.room->render(phongShader);
}
