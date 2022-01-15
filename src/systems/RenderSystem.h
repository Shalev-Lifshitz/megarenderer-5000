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

    static glm::mat4x4 getModelToWorldMatrix(glm::vec3 meshPosition);

    static glm::mat4x4 getCameraMatrix(glm::vec3 cameraOrientation);

    static glm::mat4x4 getRotationMatrixAroundX(float theta);

    static glm::mat4x4 getRotationMatrixAroundY(float theta);

    static glm::mat4x4 getRotationMatrixAroundZ(float theta);

    static glm::mat3x4 performProjection(glm::mat4x4 matProjection, glm::mat3x4 tri);

    static glm::mat4x4 getProjectionMatrix();

    static glm::mat4x4 getScalingMatrix(glm::vec3 scaleVector);

    static bool isInFrustum(glm::mat3x4 triangle, glm::vec3 cameraPosition);

    static bool isInViewNear(glm::mat3x4 triangle, glm::vec3 cameraPosition);

    static bool isInViewY(glm::mat3x4 triangle, glm::vec3 cameraPosition);

    static bool isInViewX(glm::mat3x4 triangle, glm::vec3 cameraPosition);

    static bool isInViewFar(glm::mat3x4 triangle, glm::vec3 cameraPosition);

    static void FillTriangles(std::unique_ptr<cv::Mat> &imageBackground,
                              int x0, int y0, int x1, int y1, int x2, int y2,
                              const Color &color);

    static void DrawLine(std::unique_ptr<cv::Mat> &imageBackground,
                         int x0, int y0, int x1, int y1,
                         const Color& colour);

    static void DrawTriangle(std::unique_ptr<cv::Mat> &imageBackground,
                             int x0, int y0, int x1, int y1, int x2, int y2,
                             const Color& colour);

public:
    RenderSystem(
            EntitySystem &entitySystem1,
            CameraSystem &cameraSystem1,
            int screenHeight1,
            int screenWidth1);

    std::unique_ptr<cv::Mat> renderScene(cv::Mat &imageBackground);
};

#endif //PSR_3D_RENDERER_RENDERSYSTEM_H
