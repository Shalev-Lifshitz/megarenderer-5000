#include "RenderSystem.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/ext.hpp>
#include "../math/LinearAlgebraMath.h"


RenderSystem::RenderSystem(
        EntitySystem &entitySystem1,
        CameraSystem &cameraSystem1,
        int screenHeight1,
        int screenWidth1)
        : cameraSystem(cameraSystem1), entitySystem(entitySystem1), screenHeight(screenHeight1),
          screenWidth(screenWidth1) {}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat &imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

    glm::vec3 cameraPosition = cameraSystem.getCameraPosition();
    glm::vec3 cameraOrientation = cameraSystem.getCameraOrientation();

    // These matrices can be defined outside of the loop
    glm::mat4x4 projectionMatrix = getProjectionMatrix();
    glm::mat4x4 cameraMatrix = LinearAlgebraMath::getMatrixToRotateAtoB(glm::vec3(0, 0, 1), cameraOrientation);
    cameraMatrix = glm::translate(cameraMatrix, -cameraPosition);

    // TODO: How come glm::translate works but this doesn't
//    cameraMatrix[3][0] = -cameraPosition.x;
//    cameraMatrix[3][1] = -cameraPosition.y;
//    cameraMatrix[3][2] = -cameraPosition.z;

    // TODO Orientation of mesh in model coords (before anything else).
    Meshes::const_iterator meshIter;
    for (meshIter = entitySystem.getMeshesIterBegin(); meshIter != entitySystem.getMeshesIterEnd(); meshIter++) {
        // Get entity components from entity system
        EntityID id = meshIter->first;
        Mesh entityMesh = meshIter->second;
        Position entityPosition = entitySystem.getPositionWithID(id);
        Orientation entityOrientation = entitySystem.getOrientationWithID(id);
        Scale entityScale = entitySystem.getScaleWithID(id);
        Color entityColor = entitySystem.getColorWithID(id);

        // These matrices need entity info to define
        glm::mat4x4 modelToWorldMatrix = getModelToWorldMatrix(entityPosition);
        glm::mat4x4 scalingMatrix = getScalingMatrix(entityScale);
        glm::mat4x4 finalMatrix = projectionMatrix * cameraMatrix * cameraMatrix * modelToWorldMatrix * scalingMatrix;

        bool printMatrices = true;
        if (printMatrices) {
            std::cout << std::endl;
            std::cout << "scalingMatrix: " << glm::to_string(scalingMatrix) << std::endl;
            std::cout << "modelToWorldMatrix: " << glm::to_string(modelToWorldMatrix) << std::endl;
            std::cout << "cameraMatrix: " << glm::to_string(cameraMatrix) << std::endl;
            std::cout << "projectionMatrix: " << glm::to_string(projectionMatrix) << std::endl;
            std::cout << "finalMatrix: " << glm::to_string(finalMatrix) << std::endl;
            std::cout << std::endl;
        }

        for (glm::mat3x4 tri: entityMesh) {
            glm::mat3x4 triFinal = finalMatrix * tri;

            float w0 = triFinal[0][3];
            float w1 = triFinal[1][3];
            float w2 = triFinal[2][3];
            if (w0 != 0) {triFinal[0] /= abs(w0); }
            if (w1 != 0) {triFinal[1] /= abs(w1); }
            if (w2 != 0) {triFinal[2] /= abs(w2); }

            triFinal[0][0] += 1;
            triFinal[0][1] += 1;
            triFinal[1][0] += 1;
            triFinal[1][1] += 1;
            triFinal[2][0] += 1;
            triFinal[2][1] += 1;

            triFinal[0][0] *= 0.5f * (float) screenWidth;
            triFinal[0][1] *= 0.5f * (float) screenHeight;
            triFinal[1][0] *= 0.5f * (float) screenWidth;
            triFinal[1][1] *= 0.5f * (float) screenHeight;
            triFinal[2][0] *= 0.5f * (float) screenWidth;
            triFinal[2][1] *= 0.5f * (float) screenHeight;

//            FillTriangles(image,
//                          triFinal[0][0], triFinal[0][1],
//                          triFinal[1][0], triFinal[1][1],
//                          triFinal[2][0], triFinal[2][1],
//                          entityColor);

            DrawTriangle(image,
                         triFinal[0][0], triFinal[0][1],
                         triFinal[1][0], triFinal[1][1],
                         triFinal[2][0], triFinal[2][1],
                         entityColor);
        }
    }
    return image;
}

void RenderSystem::FillTriangles(std::unique_ptr<cv::Mat> &imageBackground,
                                 int x0, int y0, int x1, int y1, int x2, int y2,
                                 const Color &color) {
    std::vector<cv::Point> pts = {cv::Point(x0, y0), cv::Point(x1, y1), cv::Point(x2, y2)};
    cv::fillPoly(*imageBackground, pts, color);
}

void RenderSystem::DrawLine(std::unique_ptr<cv::Mat> &imageBackground,
                            int x0, int y0, int x1, int y1,
                            const Color &colour) {
    if (imageBackground != nullptr) {
        cv::line(*imageBackground, {x0, y0}, {x1, y1}, colour);
    } else {
        std::cout << "Pointer to image is a null pointer!" << std::endl;
    }
}

void RenderSystem::DrawTriangle(std::unique_ptr<cv::Mat> &imageBackground,
                                int x0, int y0, int x1, int y1, int x2, int y2,
                                const Color &colour) {
    DrawLine(imageBackground, x0, y0, x1, y1, colour);
    DrawLine(imageBackground, x1, y1, x2, y2, colour);
    DrawLine(imageBackground, x2, y2, x0, y0, colour);
}

glm::mat3x4 RenderSystem::performProjection(glm::mat4x4 matProjection, glm::mat3x4 tri) {
    glm::mat3x4 triProjected = matProjection * tri;
    float w0 = glm::dot(tri[0], glm::row(matProjection, 3));
    float w1 = glm::dot(tri[1], glm::row(matProjection, 3));
    float w2 = glm::dot(tri[2], glm::row(matProjection, 3));

    // TODO: What to do when w is 0???
    //To fix the problem where models behind the camera were being inverted and re-rendered, we took the inverse.
    //What the inverse does is prevent us from dividing by negative z value, when our z values becomes negative.
    if (w0 != 0) { triProjected[0] /= w0; }
    if (w1 != 0) { triProjected[1] /= w1; }
    if (w2 != 0) { triProjected[2] /= w2; }

    return triProjected;
}

glm::mat4x4 RenderSystem::getProjectionMatrix() {

//    glm::mat4x4 mat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    float zNear = 0.1;
    float zFar = 100;

    float fovX = 45;
    float fovY = 45;

    glm::mat4x4 mat(0);
    mat[0][0] = 1 / tan(fovX / 2);
    mat[1][1] = 1 / tan(fovY / 2);
    mat[2][2] = -((zFar + zNear) / (zFar - zNear));
    mat[2][3] = -1;
    mat[3][2] = -((2 * zNear * zFar) / (zFar - zNear));

    return mat;
}

glm::mat4x4 RenderSystem::getCameraMatrix(glm::vec3 cameraOrientation) {
    glm::mat4x4 matRotX = getRotationMatrixAroundX(cameraOrientation[0]);
    glm::mat4x4 matRotY = getRotationMatrixAroundY(cameraOrientation[1]);
    glm::mat4x4 matRotZ = getRotationMatrixAroundZ(cameraOrientation[2]);
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

// Makes sure that a model is not rendered when to the right or left the screen limits
bool RenderSystem::isInViewX(glm::mat3x4 triangle, glm::vec3 cameraPosition) {
    bool inViewX = abs(triangle[0][0] - cameraPosition.x) > 2 and
                   abs(triangle[1][0] - cameraPosition.x) > 2 and
                   abs(triangle[2][0] - cameraPosition.x) > 2;
    return inViewX;
}

// Makes sure that a model is not rendered when above or below the screen limits
bool RenderSystem::isInViewY(glm::mat3x4 triangle, glm::vec3 cameraPosition) {
    bool inViewY = abs(triangle[0][1] - cameraPosition.y) > 2 and
                   abs(triangle[1][1] - cameraPosition.y) > 2 and
                   abs(triangle[2][1] - cameraPosition.y) > 2;
    return inViewY;
}

// Makes sure that a model is not rendered when closer than the near sight limit
bool RenderSystem::isInViewNear(glm::mat3x4 triangle, glm::vec3 cameraPosition) {
    bool inViewNear = triangle[0][2] + 1 > cameraPosition.z and
                      triangle[1][2] + 1 > cameraPosition.z and
                      triangle[2][2] + 1 > cameraPosition.z;
    return inViewNear;
}

// Makes sure that a model is not rendered when further than the far sight limit
bool RenderSystem::isInViewFar(glm::mat3x4 triangle, glm::vec3 cameraPosition) {
    bool inViewFar = triangle[0][2] + 1 > 2.02 and
                     triangle[1][2] + 1 > 2.02 and
                     triangle[2][2] + 1 > 2.02;
    return inViewFar;
}

// Checks that a specific triangle making up a model is within the view frustum x, y and z limits
bool RenderSystem::isInFrustum(glm::mat3x4 triangle, glm::vec3 cameraPosition) {
    bool inFrustum = isInViewX(triangle, cameraPosition) and
                     isInViewY(triangle, cameraPosition) and
                     isInViewNear(triangle, cameraPosition) and
                     isInViewFar(triangle, cameraPosition);
    return inFrustum;
}

