#include <opencv2/opencv.hpp>

#include "Game.h"
#include "MouseCallback.cpp"


Game::Game(CameraSystem cameraSystem,
           EntitySystem entitySystem,
           RenderSystem renderSystem)
        : renderSystem(cameraSystem, entitySystem) { // TODO: WHAT???? why is this needed to compile?
    this->cameraSystem = cameraSystem;
    this->entitySystem = entitySystem;
}

bool Game::runGameLoop(std::string backgroundImagePath) {
    cv::Mat imageBackground;
    imageBackground = cv::imread(backgroundImagePath, cv::IMREAD_UNCHANGED);

    if (!imageBackground.data) {
        std::cout << "Could not open or find the background image" << std::endl;
        return false;
    }

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Display window", MouseCallback, NULL); // TODO: how can we get mousePosition from this

    int key;
    while (key != 27) {
        renderSystem.renderScene(imageBackground);

        cv::imshow("Display window", imageBackground);

        key = cv::waitKey(1);
        std::cout << "Key pressed: " << key << std::endl;
        // TODO: Need to use actual mouse position from MouseCallback, we need to figure that out.
        cameraSystem.updateCamera(key, glm::vec2(0, 0));
        entitySystem.updateGame(key);
    }

    return true;
}


