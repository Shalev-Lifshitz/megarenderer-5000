#include "Camera.h"


Camera::Camera() {
    position = glm::vec3(0, 0, 0);
    orientation = glm::vec3(0, 1, 0);
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getOrientation() {
    return orientation;
}

void Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void Camera::setOrientation(glm::vec3 newOrientation) {
    orientation = newOrientation;
}