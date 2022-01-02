#include <opencv2/opencv.hpp>
#include "Game.h"

int main(int argc, char **argv) {
    std::string backgroundImagePath = "../images/gradient.jpeg";

    CameraSystem cameraSystem;
    EntitySystem entitySystem(cameraSystem);
    RenderSystem renderSystem(cameraSystem, entitySystem);
    Game game(cameraSystem, entitySystem, renderSystem);
    bool success = game.runGameLoop(backgroundImagePath);

    if (success) { return 0; }
    else { return -1; }
}