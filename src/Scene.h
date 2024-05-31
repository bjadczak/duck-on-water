//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_SCENE_H
#define PUMAROBOT_SCENE_H


#include "camera/CameraAnchorFree.h"
#include "app/AppContext.h"

class Scene {

    Shader waterShader;
    Shader whiteShader;
    Shader phongShader;
    Shader skyboxShader;
    Shader pointShader;

    AppContext &appContext;

    void setupPhong(PointLight &pointLight);
    void setupWaterShader(PointLight &pointLight);
    void drawRoom();
    void drawWater();

    void drawSkybox();
    void drawPointLight(Point &point);

public:
    explicit Scene(AppContext &appContext);
    void update();
    void render();
};


#endif //PUMAROBOT_SCENE_H
