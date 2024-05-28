//
// Created by faliszewskii on 18.04.24.
//

#include "Scene.h"


Scene::Scene(AppContext &appContext) :
        whiteShader("../res/shaders/basic/position.vert", "../res/shaders/basic/white.frag"),
        phongShader("../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag"),
        pbrShader("../res/shaders/pbr/pbr.vert", "../res/shaders/pbr/pbr.frag"),
        shadowShader("../res/shaders/shadow/shadow.vert", "../res/shaders/shadow/shadow.geom","../res/shaders/shadow/shadow.frag"),
        skyboxShader("../res/shaders/skybox/skybox.vert","../res/shaders/skybox/skybox.frag"),
        trailShader("../res/shaders/trail/trail.vert","../res/shaders/trail/trail.geom", "../res/shaders/trail/trail.frag"),
        pointShader("../res/shaders/point/point.vert","../res/shaders/point/point.frag"),
        flameShader("../res/shaders/flame/flame.vert", "../res/shaders/flame/flame.geom","../res/shaders/flame/flame.frag"),
        sparkShader("../res/shaders/spark/spark.vert", "../res/shaders/spark/spark.geom","../res/shaders/spark/spark.frag"),
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
