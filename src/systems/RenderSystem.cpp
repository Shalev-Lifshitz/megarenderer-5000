#include "RenderSystem.h"

RenderSystem::RenderSystem(
        CameraSystem& cameraSystem1,
        EntitySystem& entitySystem1)
        : entitySystem(cameraSystem1) { // TODO: WHAT???? why is this needed even when passing by ref?
    cameraSystem = cameraSystem1;
    entitySystem = entitySystem1;
}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat& imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());
    //TODO
    return image;
}
