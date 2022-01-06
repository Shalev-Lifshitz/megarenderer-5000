#include <opencv2/opencv.hpp>
#include "Game.h"

int main(int argc, char **argv) {
    std::string backgroundImagePath = "./images/gradient.jpeg";
    int cameraViewAngle = 90;
    int screenHeight = 800;
    int screenWidth = 800;

    CameraSystem cameraSystem(cameraViewAngle);
    EntitySystem entitySystem(cameraSystem);
    RenderSystem renderSystem(entitySystem, cameraSystem, screenHeight, screenWidth);
    Game game(cameraSystem, entitySystem, renderSystem);
    bool success = game.runGameLoop(backgroundImagePath, screenHeight, screenWidth);

    if (success) { return 0; }
    else { return -1; }
}