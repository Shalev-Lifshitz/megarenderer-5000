#include "RenderSystem.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext.hpp>

void DrawLine(std::unique_ptr<cv::Mat> &imageBackground, int x0, int y0, int x1, int y1, int colour) {
    if (imageBackground != nullptr) {
        cv::line(*imageBackground, {x0, y0}, {x1, y1}, colour);
    } else {
        std::cout << "Pointer to image is a null pointer!" << std::endl;
    }
}

void DrawTriangle(std::unique_ptr<cv::Mat> &imageBackground,
                  int x0, int y0, int x1, int y1, int x2, int y2, int colour) {
    DrawLine(imageBackground, x0, y0, x1, y1, colour);
    DrawLine(imageBackground, x1, y1, x2, y2, colour);
    DrawLine(imageBackground, x2, y2, x0, y0, colour);
}

RenderSystem::RenderSystem(
        EntitySystem &entitySystem1,
        CameraSystem &cameraSystem1,
        int screenHeight1,
        int screenWidth1)
        : cameraSystem(cameraSystem1), entitySystem(entitySystem1), screenHeight(screenHeight1),
          screenWidth(screenWidth1) {}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat &imageBackground, long long int elapsed_time) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

    auto thetaTime = 1.0f * elapsed_time;

    // These matrices can be defined outside of the loop
    glm::mat4x4 matProjection = getProjectionMatrix();
    glm::mat4x4 matCameraRotation = getCameraRotationMatrix(0, 0, 0);

    // TODO Orientation of mesh in model coords (before anything else).
    for (auto const &pair: entitySystem.getMeshes()) {
        std::vector<glm::mat3x4> mesh = pair.second;
        glm::vec3 meshPosition = entitySystem.getPositions().at(pair.first);
        float meshScale = entitySystem.getScales().at(pair.first);

        // These matrices need entity info to define
        glm::mat4x4 matModelToWorld = getModelToWorldMatrix(meshPosition);
        glm::mat4x4 matScaling = getScalingMatrix(glm::vec3(meshScale));

        for (glm::mat3x4 tri: mesh) {
            // Order of operations:
            // PerspectiveProjection * CameraRotation * ModelToWorld * Scaling

            glm::mat4x4 matBeforeProjection = matCameraRotation * matModelToWorld * matScaling;
            glm::mat3x4 triBeforeProjection = matBeforeProjection * tri;
            glm::mat3x4 triProjected = performProjection(matProjection, triBeforeProjection);

            DrawTriangle(image,
                         triProjected[0][0], triProjected[0][1],
                         triProjected[1][0], triProjected[1][1],
                         triProjected[2][0], triProjected[2][1],
                         0x0000);
        }
    }
    return image;
}

glm::mat3x4 RenderSystem::performProjection(glm::mat4x4 matProjection, glm::mat3x4 tri) {
    glm::mat3x4 triProjected = matProjection * tri;
    float w0 = glm::dot(tri[0], glm::row(matProjection, 3));
    float w1 = glm::dot(tri[1], glm::row(matProjection, 3));
    float w2 = glm::dot(tri[2], glm::row(matProjection, 3));

    // TODO: What to do when w is 0???
    if (w0 != 0) { triProjected[0] /= w0; }
    if (w1 != 0) { triProjected[1] /= w1; }
    if (w2 != 0) { triProjected[2] /= w2; }

    return triProjected;
}

glm::mat4x4 RenderSystem::getProjectionMatrix() {
    float zNear = 0.1;
    float zFar = 1000;

    float fovX = 90;
    float fovY = 90;

    glm::mat4x4 mat(0);
    mat[0][0] = 1 / tan(fovX / 2);
    mat[1][1] = 1 / tan(fovY / 2);
    mat[2][2] = -((zFar + zNear) / (zFar - zNear));
    mat[2][3] = -1;
    mat[3][2] = -((2 * zNear * zFar) / (zFar - zNear));

    // MOGTABA VERSION:
//    float fNear = 0.1f;
//    float fFar = 1000.0f;
//    float fFov = 90.0f;
//    float fAspectRatio = (float) screenHeight / (float) screenWidth;
//    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
//
//    glm::mat4x4 mat;
//    mat[0][0] = fAspectRatio * fFovRad;
//    mat[0][1] = 0;
//    mat[0][2] = 0;
//    mat[0][3] = 0;
//    mat[1][1] = fFovRad;
//    mat[1][0] = 0;
//    mat[1][2] = 0;
//    mat[1][3] = 0;
//    mat[2][0] = 0;
//    mat[2][1] = 0;
//    mat[2][2] = zFar / (zFar - zNear);
//    mat[2][3] = 1;
//    mat[3][0] = 0;
//    mat[3][1] = 0;
//    mat[3][2] = (-zFar * zNear) / (zFar - zNear);
//    mat[3][3] = 0;
    return mat;
}

glm::mat4x4 RenderSystem::getCameraRotationMatrix(float thetaX, float thetaY, float thetaZ) {
    glm::mat4x4 matRotX = getRotationMatrixAroundX(thetaX);
    glm::mat4x4 matRotY = getRotationMatrixAroundY(thetaY);
    glm::mat4x4 matRotZ = getRotationMatrixAroundZ(thetaZ);
    glm::mat4x4 matRot = matRotZ * matRotY * matRotX;
    return matRot;
}

glm::mat4x4 RenderSystem::getRotationMatrixAroundX(float theta) {
    glm::mat4x4 mat(1);
    mat[1][1] = cos(theta);
    mat[1][2] = sin(theta);
    mat[2][1] = -sin(theta);
    mat[2][2] = cos(theta);
    return mat;
}

glm::mat4x4 RenderSystem::getRotationMatrixAroundY(float theta) {
    glm::mat4x4 mat(1);
    mat[0][0] = cos(theta);
    mat[0][2] = -sin(theta);
    mat[2][0] = sin(theta);
    mat[2][2] = cos(theta);
    return mat;
}

glm::mat4x4 RenderSystem::getRotationMatrixAroundZ(float theta) {
    glm::mat4x4 mat(1);
    mat[0][0] = cos(theta);
    mat[0][1] = sin(theta);
    mat[1][0] = -sin(theta);
    mat[1][1] = cos(theta);
    return mat;
}

glm::mat4x4 RenderSystem::getModelToWorldMatrix(glm::vec3 meshPosition) {
    return glm::translate(glm::mat4(1.0f), meshPosition);
}

glm::mat4x4 RenderSystem::getScalingMatrix(glm::vec3 scaleVector) {
    glm::mat4x4 mat(1);
    mat[0][0] = scaleVector.x;
    mat[1][1] = scaleVector.y;
    mat[2][2] = scaleVector.z;
    return mat;
}
