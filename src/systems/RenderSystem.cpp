#include "RenderSystem.h"

RenderSystem::RenderSystem(CameraSystem cameraSystem, EntitySystem entitySystem) {
    this->entitySystem = entitySystem; // TODO: This is copying... BAD.
    this->cameraSystem = cameraSystem; // TODO: This is copying... BAD.
}

cv::Mat RenderSystem::renderScene(cv::Mat imageBackground) {
    return imageBackground;
    //TODO
}
