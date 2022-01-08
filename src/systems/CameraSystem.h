#ifndef PSR_3D_RENDERER_CAMERASYSTEM_H
#define PSR_3D_RENDERER_CAMERASYSTEM_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include "../entities/Camera.h"

class CameraSystem {
private:
    Camera camera;
    glm::vec3 positiveX = glm::vec3(1, 0, 0);
    glm::vec3 positiveY = glm::vec3(0, 1, 0);
    glm::vec3 positiveZ = glm::vec3(0, 0, 1);

    void moveCameraForward(float multiplier);

    void moveCameraBackward(float multiplier);

    void moveCameraRight(float multiplier);

    void moveCameraLeft(float multiplier);

    void moveCameraUp(float multiplier);

    void moveCameraDown(float multiplier);

public:
    explicit CameraSystem(int cameraViewAngle);

    void updateCamera(int keycode, glm::vec2 mousePosition);

    glm::vec3 getCameraPosition();

    void setCameraPosition(glm::vec3 newCameraPosition);

    glm::vec3 getCameraOrientation();

    void setCameraOrientation(const glm::vec2 &mousePosition);

    int getCameraViewAngle();

    void performSanityChecks();
};


#endif //PSR_3D_RENDERER_CAMERASYSTEM_H
