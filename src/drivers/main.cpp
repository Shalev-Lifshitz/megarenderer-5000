#include <opencv2/opencv.hpp>
#include "Game.h"

int main(int argc, char **argv) {
    std::string backgroundImagePath = "../images/gradient.jpeg";
    int cameraViewAngle = 90;
    CameraSystem cameraSystem(cameraViewAngle);
    EntitySystem entitySystem(cameraSystem);
    RenderSystem renderSystem(entitySystem, cameraSystem);

    int screenHeight = 1000;
    int screenWidth = 1000;
    Game game(cameraSystem, entitySystem, renderSystem);
    bool success = game.runGameLoop(backgroundImagePath, screenHeight, screenWidth);

    if (success) { return 0; }
    else { return -1; }
}