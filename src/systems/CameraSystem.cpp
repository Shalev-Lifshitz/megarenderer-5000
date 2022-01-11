#include <iostream>
#include "CameraSystem.h"
#include "glm/gtx/string_cast.hpp"
#include <opencv2/opencv.hpp>
#include "../math/LinearAlgebraMath.h"

// TODO: Check that orientation is always a unit vector
CameraSystem::CameraSystem(int cameraViewAngle) : camera(cameraViewAngle) {}

void CameraSystem::updateCamera(int keycode) {
    updateCameraPosition(keycode);
    updateCameraOrientation();
}

void CameraSystem::updateCameraPosition(int keycode) {
    switch (keycode) {
        case 119: // 'w' - FORWARD
            moveCameraForward(positionSensitivity);
            break;
        case 115: // 's' - BACKWARD
            moveCameraBackward(positionSensitivity);
            break;
        case 97: // 'a' - LEFT
            moveCameraLeft(positionSensitivity);
            break;
        case 100: // 'd' - RIGHT
            moveCameraRight(positionSensitivity);
            break;
        case 32: // 'SPACE' - UP
            moveCameraUp(positionSensitivity);
            break;
        case 9: // 'TAB' - DOWN
            moveCameraDown(positionSensitivity);
            break;
    }
}

void CameraSystem::updateCameraOrientation() {
    if (mouseIsBeingDragged) {
        // Compute orientation change vec that lies in the plane tangential to the orientation vector
        glm::vec2 mousePositionDiff = mousePositionCurr - mousePositionPrev;
        glm::vec2 orientationChange2D = -mousePositionDiff;

        // Don't update orientation if orientationChange2D is the zero vector
        if (LinearAlgebraMath::equalsEstimate(orientationChange2D, glm::vec2(0))) {
            return;
        }

        // Compute the matrix which rotates the z-axis to align with the orientation vector
        glm::mat3x3 rotationMatrix = LinearAlgebraMath::getMatrixToRotateAtoB(positiveZ, getCameraOrientation());

        // Compute basis vector in the plane tangential to the orientation vector
        glm::vec3 positiveXInPlane = (rotationMatrix * positiveX) + getCameraOrientation();
        glm::vec3 positiveYInPlane = (rotationMatrix * positiveY) + getCameraOrientation();

        // Traverse along the tangential plane in the direction of orientationChange2D
        // The traversal is relative to the magnitude of orientationChange2D and orientationSensitivity
        auto xChangeInPlane = orientationChange2D.x * positiveXInPlane;
        auto yChangeInPlane = orientationChange2D.y * positiveYInPlane;
        auto changeInPlane = orientationSensitivity * (xChangeInPlane + yChangeInPlane);
        auto newOrientationInPlane = getCameraOrientation() + changeInPlane;
        glm::vec3 newOrientationOnUnitSphere = glm::normalize(newOrientationInPlane);
        setCameraOrientation(newOrientationOnUnitSphere);
        performSanityChecks();
    }
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

void CameraSystem::setCameraOrientation(glm::vec3 newCameraOrientation) {
    camera.setOrientation(newCameraOrientation);
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
    float orientationLength = glm::length(getCameraOrientation());
    assert(LinearAlgebraMath::equalsEstimate(orientationLength, 1));
}

void CameraSystem::printCameraInfo() {
    std::cout << std::endl;
    std::cout << "Camera Position: " << glm::to_string(getCameraPosition()) << std::endl;
    std::cout << "Camera Orientation: " << glm::to_string(getCameraOrientation()) << std::endl;
    std::cout << std::endl;
}

// TODO: Why does rotation continue when I stop moving during a drag?
void CameraSystem::recordMouseEvent(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) { // Start a drag
        mouseIsBeingDragged = true;

        // mousePositionCurr and mousePositionPrev should be current position
        // at the start of a drag since we want a no-op when we first click.
        mousePositionCurr = glm::vec2(x, y);
        mousePositionPrev = mousePositionCurr;
    } else if (event == cv::EVENT_LBUTTONUP) { // End a drag
        mouseIsBeingDragged = false;
    } else if (event == cv::EVENT_MOUSEMOVE) {
        if (mouseIsBeingDragged) {
            // If we are in the middle of a drag, update prev position and set new curr.
            mousePositionPrev = mousePositionCurr;
            mousePositionCurr = glm::vec2(x, y);
        }
    }
}
