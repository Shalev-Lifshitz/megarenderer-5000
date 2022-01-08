#include <iostream>
#include "CameraSystem.h"
#include "glm/gtx/string_cast.hpp"

// TODO: Check that orientation is always a unit vector
CameraSystem::CameraSystem(int cameraViewAngle)
        : camera(cameraViewAngle) {}

void CameraSystem::updateCamera(int keycode, glm::vec2 mousePosition) {
    float multiplier = 1;
    switch (keycode) {
        case 119: // 'w' - FORWARD
            moveCameraForward(multiplier);
            break;
        case 115: // 's' - BACKWARD
            moveCameraBackward(multiplier);
            break;
        case 97: // 'a' - LEFT
            moveCameraLeft(multiplier);
            break;
        case 100: // 'd' - RIGHT
            moveCameraRight(multiplier);
            break;
        case 32: // 'SPACE' - UP
            moveCameraUp(multiplier);
            break;
        case 9: // 'TAB' - DOWN
            moveCameraDown(multiplier);
            break;
    }

    // TODO: Update camera orientation
    // setCameraOrientation(mousePosition);
}


glm::vec3 CameraSystem::getCameraPosition() {
    return camera.getPosition();
}

void CameraSystem::setCameraPosition(glm::vec3 newCameraPosition) {
    camera.setPosition(newCameraPosition);
}

glm::vec3 CameraSystem::getCameraOrientation() {
    return camera.getOrientation();
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

int CameraSystem::getCameraViewAngle() {
    return camera.getCameraViewAngle();
}

void CameraSystem::moveCameraForward(float multiplier) {
    glm::vec3 positionUpdate = multiplier * camera.getOrientation();
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::moveCameraBackward(float multiplier) {
    glm::vec3 positionUpdate = multiplier * -camera.getOrientation();
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::moveCameraRight(float multiplier) {  // TODO: -nan when current orientation = positiveY. Fix.
    glm::vec3 positionUpdate = multiplier * glm::normalize(glm::cross(positiveY, getCameraOrientation()));
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::moveCameraLeft(float multiplier) {  // TODO: nan when current orientation = positiveY. Fix.
    glm::vec3 positionUpdate = multiplier * -glm::normalize(glm::cross(positiveY, getCameraOrientation()));
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::moveCameraUp(float multiplier) {
    glm::vec3 positionUpdate = multiplier * positiveY;
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::moveCameraDown(float multiplier) {
    glm::vec3 positionUpdate = multiplier * -positiveY;
    glm::vec3 newCameraPosition = getCameraPosition() + positionUpdate;
    setCameraPosition(newCameraPosition);
}

void CameraSystem::performSanityChecks() {
    // Check that camera orientation vector is a unit vector
    assert(glm::length(getCameraOrientation()) == 1);
}

void CameraSystem::printCameraInfo() {
    std::cout << std::endl;
    std::cout << "Camera Position: " << glm::to_string(getCameraPosition()) << std::endl;
    std::cout << "Camera Orientation: " << glm::to_string(getCameraOrientation()) << std::endl;
    std::cout << std::endl;
}

