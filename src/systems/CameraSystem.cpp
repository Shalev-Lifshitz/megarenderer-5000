#include <iostream>
#include "CameraSystem.h"
#include "glm/gtx/string_cast.hpp"

CameraSystem::CameraSystem(int cameraViewAngle)
        : camera(cameraViewAngle) {}

void CameraSystem::updateCamera(int keycode, glm::vec2 mousePosition) {
    // Determine how to update camera position based on key pressed
    bool updatedPosition = false;
    float mult = 0.05;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 positionUpdate;
    switch (keycode) {
        case 119: // 'w' - FORWARD
            positionUpdate = mult * cameraFront;
            updatedPosition = true;
            break;
        case 115: // 's' - BACKWARD
            positionUpdate = -mult * cameraFront;
            updatedPosition = true;
            break;
        case 97: // 'a' - LEFT
            positionUpdate = glm::normalize(glm::cross(cameraFront, cameraUp)) * mult;
            updatedPosition = true;
            break;
        case 100: // 'd' - RIGHT
            positionUpdate = glm::normalize(glm::cross(cameraFront, cameraUp)) * -mult;
            updatedPosition = true;
            break;
        case 32: // 'SPACE' - UP
            positionUpdate = glm::vec3(0, 1 * mult, 0);
            updatedPosition = true;
            break;
        case 9: // 'TAB' - DOWN
            positionUpdate = glm::vec3(0, -1 * mult, 0);
            updatedPosition = true;
            break;
    }
    if (updatedPosition) {
        // Compute new position and update camera
        glm::vec3 newCameraPosition = camera.getPosition() + positionUpdate;
        camera.setPosition(newCameraPosition);
    }


    // TODO: Update camera orientation
//    setCameraOrientation(mousePosition);
}

void CameraSystem::setCameraOrientation(const glm::vec2 &mousePosition) {
    // adding constants to change later (might be picture specific currently?)
    float xScreenMidpoint = 500;
    float yScreenMidpoint = 327.5;
    float movementScalar = 10;

    // defined area at the centre of the screen where the cursor does not move the camera orientation
    bool xPosNeutral = 400 < mousePosition[0] && mousePosition[0] < 600;
    bool yPosNeutral = 277 < mousePosition[1] && mousePosition[1] < 377;
    // when the cursor is in the centre of the screen.
    if (xPosNeutral && yPosNeutral) {
        std::cout << "NO ORIENTATION UPDATE";
    } else {
        // we want to move the orientation
        // calculate norm of vector with origin at screen midpoint
        float xDist = std::abs(mousePosition[0] - xScreenMidpoint);
        float yDist = std::abs(mousePosition[1] - yScreenMidpoint);
        std::cout << "ORIENTATION UPDATED BY (" << xDist << ", " << yDist << ")";
    }
}

glm::vec3 CameraSystem::getCameraPosition() {
    return camera.getPosition();
}

glm::vec3 CameraSystem::getCameraOrientation() {
    return camera.getOrientation();
}

int CameraSystem::getCameraViewAngle() {
    return camera.getCameraViewAngle();
}
