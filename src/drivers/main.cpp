#include <opencv2/opencv.hpp>
#include "Game.h"
#include "../math/TestLinearAlgebraMath.h"
#include "glm/gtx/vector_angle.hpp"

int main(int argc, char **argv) {
    bool runTests = false;
    if (runTests) {
        TestLinearAlgebraMath::testAll();
    } else {
        std::string backgroundImagePath = "./images/3d-mesh.jpg";
        int cameraViewAngle = 90;
        int screenHeight = 800;
        int screenWidth = 800;

        CameraSystem cameraSystem(cameraViewAngle, screenHeight, screenWidth);
        EntitySystem entitySystem(cameraSystem);
        RenderSystem renderSystem(entitySystem, cameraSystem, screenHeight, screenWidth);
        Game game(cameraSystem, entitySystem, renderSystem);
        bool success = game.runGameLoop(backgroundImagePath, screenHeight, screenWidth);

        if (success) { return 0; }
        else { return -1; }
    }
}