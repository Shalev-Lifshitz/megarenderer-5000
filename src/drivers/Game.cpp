#include <opencv2/opencv.hpp>
//#include <chrono>

#include "glm/gtx/string_cast.hpp"
#include "Game.h"

void MouseCallback(int event, int x, int y, int flags, void *userdata) {
    Game *game = static_cast<Game*>(userdata);
    game->recordMouseEvent(event, x, y, flags);
}

Game::Game(CameraSystem &cameraSystem1,
           EntitySystem &entitySystem1,
           RenderSystem &renderSystem1)
        : cameraSystem(cameraSystem1),
          entitySystem(entitySystem1),
          renderSystem(renderSystem1) {}

bool Game::runGameLoop(std::string backgroundImagePath, int screenHeight, int screenWidth) {
    cv::Mat imageBackground;
    imageBackground = cv::imread(backgroundImagePath, cv::IMREAD_UNCHANGED);

    if (!imageBackground.data) {
        std::cout << "Could not open or find the background image" << std::endl;
        return false;
    }

    // Resize screen
    cv::resize(imageBackground, imageBackground, cv::Size(screenWidth, screenHeight));

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Display window", MouseCallback, this);

    int key;
    std::unique_ptr<cv::Mat> image;
//    auto start = std::chrono::steady_clock::now();
    bool printDebugInfo = false;
    while (key != 27) {
        cameraSystem.performSanityChecks();

        if (printDebugInfo) {
            cameraSystem.printCameraInfo();
            std::cout << "Mouse Position: " << glm::to_string(mousePosition) << std::endl;
        }

//        auto end = std::chrono::steady_clock::now();
//        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        image = renderSystem.renderScene(imageBackground);

        cv::imshow("Display window", *image);

        key = cv::waitKey(1);

        cameraSystem.updateCamera(key);
        entitySystem.updateGame(key);
    }
    return true;
}

void Game::recordMouseEvent(int event, int x, int y, int flags) {
    cameraSystem.recordMouseEvent(event, x, y, flags);
}


