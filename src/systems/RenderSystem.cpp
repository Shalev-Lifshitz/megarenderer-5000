#include "RenderSystem.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext.hpp>

void FillTriangles(std::unique_ptr<cv::Mat> &imageBackground,
                   int x0, int y0, int x1, int y1, int x2, int y2, cv::Scalar_<double> color){
    std::vector<cv::Point> pts = {cv::Point(x0, y0), cv::Point(x1, y1), cv::Point(x2, y2)};
    cv::fillPoly(*imageBackground, pts, color);
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
        auto color = entitySystem.getColors().at(pair.first);

        // These matrices need entity info to define
        glm::mat4x4 matModelToWorld = getModelToWorldMatrix(meshPosition);
        glm::mat4x4 matScaling = getScalingMatrix(glm::vec3(meshScale));

        bool printMatrices = false;
        if (printMatrices) {
            std::cout << std::endl;
            std::cout << "matProjection: " << glm::to_string(matProjection) << std::endl;
            std::cout << "matCameraRotation: " << glm::to_string(matCameraRotation) << std::endl;
            std::cout << "matModelToWorld: " << glm::to_string(matModelToWorld) << std::endl;
            std::cout << "matScaling: " << glm::to_string(matScaling) << std::endl;
            std::cout << std::endl;
        }

        for (glm::mat3x4 tri: mesh) {
            // Order of operations:
            // PerspectiveProjection * CameraRotation * ModelToWorld * Scaling

            glm::mat3x4 triTranslated = adjustPositionZ(tri, meshPosition, cameraSystem.getCameraPosition());
            glm::mat4x4 matBeforeProjection = matCameraRotation * matModelToWorld * matScaling;
            glm::mat3x4 triBeforeProjection = matBeforeProjection * triTranslated;
            glm::mat3x4 triPreProjected = performProjection(matProjection, triBeforeProjection);
            glm::mat3x4 triProjected = adjustPositionXY(triPreProjected, meshPosition, cameraSystem.getCameraPosition());


            FillTriangles(image,
                          triProjected[0][0], triProjected[0][1],
                          triProjected[1][0], triProjected[1][1],
                          triProjected[2][0], triProjected[2][1],
                          color);
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

glm::mat3x4 RenderSystem::adjustPositionXY(glm::mat3x4 triangle, glm::vec3 meshPosition, glm::vec3 cameraPosition) {

    glm::mat3x4 triAdjustedXY = triangle;

    triAdjustedXY[0][0] += cameraSystem.getCameraPosition().x + 1.0f;
    triAdjustedXY[0][1] += cameraSystem.getCameraPosition().y + 1.0f;
    triAdjustedXY[1][0] += cameraSystem.getCameraPosition().x + 1.0f;
    triAdjustedXY[1][1] += cameraSystem.getCameraPosition().y + 1.0f;
    triAdjustedXY[2][0] += cameraSystem.getCameraPosition().x + 1.0f;
    triAdjustedXY[2][1] += cameraSystem.getCameraPosition().y + 1.0f;

    triAdjustedXY[0][0] += meshPosition.x;
    triAdjustedXY[0][1] += meshPosition.y;
    triAdjustedXY[1][0] += meshPosition.x;
    triAdjustedXY[1][1] += meshPosition.y;
    triAdjustedXY[2][0] += meshPosition.x;
    triAdjustedXY[2][1] += meshPosition.y;

    triAdjustedXY[0][0] *= 0.5f * (float) screenWidth;
    triAdjustedXY[0][1] *= 0.5f * (float) screenHeight;
    triAdjustedXY[1][0] *= 0.5f * (float) screenWidth;
    triAdjustedXY[1][1] *= 0.5f * (float) screenHeight;
    triAdjustedXY[2][0] *= 0.5f * (float) screenWidth;
    triAdjustedXY[2][1] *= 0.5f * (float) screenHeight;


    return triAdjustedXY;
}

glm::mat3x4 RenderSystem::adjustPositionZ(glm::mat3x4 triangle, glm::vec3 meshPosition, glm::vec3 cameraPosition) {

    glm::mat3x4 triAdjustedZ = triangle;
    triAdjustedZ[0][2] = triangle[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
    triAdjustedZ[1][2] = triangle[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
    triAdjustedZ[2][2] = triangle[2][2] + 3.0f + cameraSystem.getCameraPosition().z;


    triAdjustedZ[0][2] += meshPosition.z;
    triAdjustedZ[1][2] += meshPosition.z;
    triAdjustedZ[2][2] += meshPosition.z;


    return triAdjustedZ;
}
