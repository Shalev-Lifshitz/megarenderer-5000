#ifndef PSR_3D_RENDERER_CAMERASYSTEM_H
#define PSR_3D_RENDERER_CAMERASYSTEM_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include "../entities/Camera.h"

class CameraSystem {
private:
    Camera camera;
public:
    explicit CameraSystem(int cameraViewAngle);

    void updateCamera(int keycode, glm::vec2 mousePosition);

    glm::vec3 getCameraPosition();

    glm::vec3 getCameraOrientation();

    int getCameraViewAngle();
};


#endif //PSR_3D_RENDERER_CAMERASYSTEM_H
