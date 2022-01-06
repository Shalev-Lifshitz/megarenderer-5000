#include <opencv2/opencv.hpp>
#include <chrono>

#include "Game.h"

void MouseCallback(int event, int x, int y, int flags, void *userdata) {
//    if (event == cv::EVENT_LBUTTONDOWN) {
//        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
//    } else if (event == cv::EVENT_RBUTTONDOWN) {
//        std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
//    } else if (event == cv::EVENT_MBUTTONDOWN) {
//        std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
//    } else if (event == cv::EVENT_MOUSEMOVE) {
//        std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;
//    }
}

Game::Game(CameraSystem& cameraSystem1,
           EntitySystem& entitySystem1,
           RenderSystem& renderSystem1)
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
    cv::setMouseCallback("Display window", MouseCallback, NULL); // TODO: how can we get mousePosition from this

    int key;
    std::unique_ptr<cv::Mat> image;
    auto start = std::chrono::steady_clock::now();
    while (key != 27) {
        auto end = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        image = renderSystem.renderScene(imageBackground, elapsed_time/100);

        cv::imshow("Display window", *image);

        key = cv::waitKey(1);
        std::cout << "Key    pressed: " << key << std::endl;
        // TODO: Need to use actual mouse position from MouseCallback, we need to figure that out.
        cameraSystem.updateCamera(key, glm::vec2(0, 0));
        entitySystem.updateGame(key);
    }

    return true;
}


