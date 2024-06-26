//
// Created by faliszewskii on 18.04.24.
//

#include "Scene.h"


Scene::Scene(AppContext &appContext) :
        waterShader("../res/shaders/water/water.vert", "../res/shaders/water/water.frag"),
        whiteShader("../res/shaders/basic/position.vert", "../res/shaders/basic/white.frag"),
        phongCubemapShader("../res/shaders/phong/phongCubeMap.vert", "../res/shaders/phong/phongCubeMap.frag"),
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
    setupPhongCubemap(appContext.pointLight);

    drawRoom();

    setupPhong(appContext.pointLight);
    drawDuck();

    drawWater();


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


void Scene::setupPhongCubemap(PointLight &pointLight) {
    phongCubemapShader.use();
    phongCubemapShader.setUniform("view", appContext.camera->getViewMatrix());
    phongCubemapShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    phongCubemapShader.setUniform("viewPos", appContext.camera->getViewPosition());
    phongCubemapShader.setUniform("isMirror", false);
    pointLight.setupPointLight(phongCubemapShader);
}

void Scene::setupPhong(PointLight &pointLight) {
    phongShader.use();
    phongShader.setUniform("view", appContext.camera->getViewMatrix());
    phongShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    phongShader.setUniform("viewPos", appContext.camera->getViewPosition());
    phongShader.setUniform("isMirror", false);
    pointLight.setupPointLight(phongShader);
}

void Scene::drawRoom() {
    appContext.room->render(phongCubemapShader);
}

void Scene::setupWaterShader (PointLight &pointLight)
{
    waterShader.use();
    waterShader.setUniform("view", appContext.camera->getViewMatrix());
    waterShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    waterShader.setUniform("viewPos", appContext.camera->getViewPosition());
    waterShader.setUniform("isMirror", false);
    pointLight.setupPointLight(waterShader);
}

void Scene::drawWater ()
{
    appContext.water->updateDuckPos(appContext.duck->getPos());
    setupWaterShader(appContext.pointLight);
    appContext.room->cubemap->bind(1);
    waterShader.setUniform("cubemapTexture", 1);
    appContext.water->render(waterShader);
}

void Scene::drawDuck ()
{
    appContext.duck->render(phongShader);
}
