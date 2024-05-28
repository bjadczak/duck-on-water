//
// Created by faliszewskii on 19.04.24.
//

#ifndef PUMAROBOT_APPCONTEXT_H
#define PUMAROBOT_APPCONTEXT_H


#include "../room/Room.h"
#include "../camera/CameraAnchorFree.h"
#include "../framebufferManager/FrameBufferManager.h"
#include "../importer/Importer.h"
#include "../light/PointLight.h"
#include "../skybox/Skybox.h"
#include "../point/Point.h"
#include "../camera/CameraGameLike.h"
#include "../waterSurface/Water.h"

struct AppContext {

    std::unique_ptr<Water> water;
    std::unique_ptr<Room> room;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<Point> light;

    PointLight pointLight;
    std::unique_ptr<BaseCamera> camera;

    std::unique_ptr<FrameBufferManager> frameBufferManager;

    bool guiFocus = true;

    CameraType cameraType;

    float cameraSensitivity = 0.8f;

    void glfw_window_resize(unsigned int width, unsigned int height)
    {
        camera->resize(width, height); // NOLINT(*-narrowing-conversions)
        frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);
    }

    void allocateCamera(CameraType setType)
    {
        cameraType = setType;
        switch(setType)
        {
            case CameraType::GAMELIKE:
                if(camera != nullptr)
                    camera.reset();
                camera = std::make_unique<CameraGameLike>(1920, 1080, CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
                break;
            case CameraType::FREEANCHOR:
                if(camera != nullptr)
                    camera.reset();
                camera = std::make_unique<CameraAnchorFree>(1920, 1080, CameraMode::ANCHOR, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
                break;

        }
    }

    AppContext() :
        frameBufferManager()
        {
            allocateCamera(CameraType::GAMELIKE);
            frameBufferManager = std::make_unique<FrameBufferManager>();
            frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);

            water = std::make_unique<Water>();
            room = std::make_unique<Room>();
            skybox = std::make_unique<Skybox>();
            light = std::make_unique<Point>();
            pointLight.position = glm::vec3(0,3.5,0);
        }
};


#endif //PUMAROBOT_APPCONTEXT_H
