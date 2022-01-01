#include "CameraSystem.h"

CameraSystem::CameraSystem() {
    camera = Camera();
}

void CameraSystem::updateCamera(int keycode, glm::vec2 mousePosition) {
    // Determine how to update camera position based on key pressed
    glm::vec3 positionUpdate;
    switch(keycode)
    {
        case 119: // 'w' - FORWARD
            positionUpdate = glm::vec3(1, 0, 0);
            break;
        case 115: // 's' - BACKWARD
            positionUpdate = glm::vec3(-1, 0, 0);
            break;
        case 97: // 'a' - LEFT
            positionUpdate = glm::vec3(0, 0, -1);
            break;
        case 100: // 'd' - RIGHT
            positionUpdate = glm::vec3(0, 0, 1);
            break;
        case 9: // 'TAB' - UP
            positionUpdate = glm::vec3(0, 1, 0);
            break;
        case 32: // 'SPACE' - DOWN
            positionUpdate = glm::vec3(0, -1, 0);
            break;
    }

    // Compute new position and update camera
    glm::vec3 newCameraPosition = camera.getPosition() + positionUpdate;
    camera.setPosition(newCameraPosition);

    // TODO: Update camera orientation
}

glm::vec3 CameraSystem::getCameraPosition() {
    return camera.getPosition();
}

glm::vec3 CameraSystem::getCameraOrientation() {
    return camera.getOrientation();
}