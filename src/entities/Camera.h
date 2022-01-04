#ifndef PSR_3D_RENDERER_CAMERA_H
#define PSR_3D_RENDERER_CAMERA_H

#include <glm/glm.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 orientation;
    int cameraViewAngle;
public:
    Camera(int cameraViewAngle);

    glm::vec3 getPosition();

    void setPosition(glm::vec3 newPosition);

    glm::vec3 getOrientation();

    void setOrientation(glm::vec3 newOrientation);

    int getCameraViewAngle();

    void setCameraViewAngle(int newCameraViewAngle);
};


#endif //PSR_3D_RENDERER_CAMERA_H
