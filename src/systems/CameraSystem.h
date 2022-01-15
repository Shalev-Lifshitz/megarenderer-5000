#ifndef PSR_3D_RENDERER_CAMERASYSTEM_H
#define PSR_3D_RENDERER_CAMERASYSTEM_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include "../entities/Camera.h"

class CameraSystem {
private:
    Camera camera;

    bool mouseIsBeingDragged = false;
    glm::vec2 mousePositionPrev = glm::vec2(0);
    glm::vec2 mousePositionCurr = glm::vec2(0);

    float orientationSensitivity = 0.001;
    float positionSensitivity = 0.1;

    glm::vec3 positiveX = glm::vec3(1, 0, 0);
    glm::vec3 positiveY = glm::vec3(0, 1, 0);
    glm::vec3 positiveZ = glm::vec3(0, 0, 1);

    void updateCameraPosition(int keycode);

    void updateCameraOrientation();

    void moveCameraForward(float multiplier);

    void moveCameraBackward(float multiplier);

    void moveCameraRight(float multiplier);

    void moveCameraLeft(float multiplier);

    void moveCameraUp(float multiplier);

    void moveCameraDown(float multiplier);

    void setCameraPosition(glm::vec3 newCameraPosition);

    void setCameraOrientation(glm::vec3 newCameraOrientation);

public:
    explicit CameraSystem(int cameraViewAngle);

    void updateCamera(int keycode);

    glm::vec3 getCameraPosition();

    glm::vec3 getCameraOrientation();

    int getCameraViewAngle();

    void performSanityChecks();

    void printCameraInfo();

    void recordMouseEvent(int event, int x, int y, int flags);
};


#endif //PSR_3D_RENDERER_CAMERASYSTEM_H
