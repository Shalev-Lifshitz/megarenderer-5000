#include "RenderSystem.h"

RenderSystem::RenderSystem(CameraSystem &cameraSystem, EntitySystem &entitySystem) {
    this->entitySystem = entitySystem;
    this->cameraSystem = cameraSystem;
}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat &imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());
    //TODO
    return image;
}
