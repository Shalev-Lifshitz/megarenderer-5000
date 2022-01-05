#include "RenderSystem.h"

RenderSystem::RenderSystem(EntitySystem& entitySystem1, CameraSystem& cameraSystem1)
    : cameraSystem(cameraSystem1), entitySystem(entitySystem1) {}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat& imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());
    //TODO
    return image;
}
