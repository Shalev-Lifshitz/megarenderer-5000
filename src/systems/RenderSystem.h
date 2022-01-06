#ifndef PSR_3D_RENDERER_RENDERSYSTEM_H
#define PSR_3D_RENDERER_RENDERSYSTEM_H


#include "CameraSystem.h"
#include "EntitySystem.h"
#include <memory.h>

class RenderSystem {
private:
    EntitySystem &entitySystem;
    CameraSystem &cameraSystem;
    int screenHeight;
    int screenWidth;

    glm::mat4x4 getModelToWorldMatrix(glm::vec3 meshPosition);

    glm::mat4x4 getCameraRotationMatrix(float thetaX, float thetaY, float thetaZ);

    glm::mat4x4 getRotationMatrixAroundX(float theta);

    glm::mat4x4 getRotationMatrixAroundY(float theta);

    glm::mat4x4 getRotationMatrixAroundZ(float theta);

    glm::mat3x4 performProjection(glm::mat4x4 matProjection, glm::mat3x4 tri);

    glm::mat4x4 getProjectionMatrix();

    glm::mat4x4 getScalingMatrix(glm::vec3 scaleVector);

public:
    RenderSystem(
            EntitySystem &entitySystem1,
            CameraSystem &cameraSystem1,
            int screenHeight1,
            int screenWidth1);

    std::unique_ptr<cv::Mat> renderScene(cv::Mat &imageBackground, long long int i);

    glm::mat3x4 adjustPositionXY(glm::mat3x4 mat1, glm::vec3 meshPosition, glm::vec3 cameraPosition);

    glm::mat3x4 adjustPositionZ(glm::mat3x4 triangle, glm::vec3 meshPosition, glm::vec3 cameraPosition);
};

#endif //PSR_3D_RENDERER_RENDERSYSTEM_H
