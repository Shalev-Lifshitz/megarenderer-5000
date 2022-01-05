#include "RenderSystem.h"
#include <tuple>
#include <utility>
#include <type_traits>
#include "glm/gtx/string_cast.hpp"
#include <glm/ext/matrix_transform.hpp>

void RenderSystem::MatrixVectorMultiplier(glm::vec4 &i, glm::vec4 &o, glm::mat4x4 &m) {
    o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + m[3][0];
    o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + m[3][1];
    o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + m[3][2];
    float w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + m[3][3];

    if (w != 0.0f)
    {
        o.x/= w; o.y/= w; o.z/= w;
    }
};

void DrawLine(std::unique_ptr<cv::Mat> &imageBackground, int x0, int y0, int x1, int y1, int colour){
    if (imageBackground != nullptr){
        cv::line(*imageBackground, {x0, y0}, {x1, y1}, colour);
    } else {
        std::cout << "Pointer to image is a null pointer!" << std::endl;
    }
};

//cv::Mat @ 0x00007fbd60b073d0

void DrawTriangle(std::unique_ptr<cv::Mat> &imageBackground, int x0, int y0, int x1, int y1, int x2, int y2, int colour){

    DrawLine(imageBackground, x0, y0, x1, y1, colour);
    DrawLine(imageBackground, x1, y1, x2, y2, colour);
    DrawLine(imageBackground, x2, y2, x0, y0, colour);

};

RenderSystem::RenderSystem(EntitySystem& entitySystem1, CameraSystem& cameraSystem1)
    : cameraSystem(cameraSystem1), entitySystem(entitySystem1) {}

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat &imageBackground, long long int elapsed_time) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

    auto screenHeight = 800;
    auto screenWidth = 800;

    for(auto const &pair: entitySystem.getMeshes()) {
        std::vector<glm::mat3x4> mesh = pair.second;
        glm::vec3 position3 = entitySystem.getPositions().at(pair.first);
        glm::vec4 position4 = glm::vec4(position3.x, position3.y, position3.z, 0);

        //Projection Matrix
        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float) screenHeight / (float) screenWidth;
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    //Scale matrix
    int meshScale = 1;


    matProj[0][0] = fAspectRatio * fFovRad;
    matProj[0][1] = 0.0f;
    matProj[0][2] = 0.0f;
    matProj[0][3] = 0.0f;
    matProj[1][1] = fFovRad;
    matProj[1][0] = 0.0f;
    matProj[1][2] = 0.0f;
    matProj[1][3] = 0.0f;
    matProj[2][0] = 0.0f;
    matProj[2][1] = 0.0f;
    matProj[2][2] = fFar / (fFar - fNear);
    matProj[2][3] = 1.0f;
    matProj[3][0] = 0.0f;
    matProj[3][1] = 0.0f;
    matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj[3][3] = 0.0f;

    glm::mat4x4 matRotZ, matRotX, matScale;
    auto fTheta = 1.0f * elapsed_time;

    // Rotation z
    matRotZ[0][0] = cosf(fTheta);
    matRotZ[0][1] = sinf(fTheta);
    matRotZ[0][2] = 0;
    matRotZ[0][3] = 0;
    matRotZ[1][0] = -sinf(fTheta);
    matRotZ[1][1] =cosf(fTheta);;
    matRotZ[1][2] = 0;
    matRotZ[1][3] = 0;
    matRotZ[2][0] = 0;
    matRotZ[2][1] = 0;
    matRotZ[2][2] = 1;
    matRotZ[2][3] = 0;
    matRotZ[3][0] = 0;
    matRotZ[3][1] = 0;
    matRotZ[3][2] = 0;
    matRotZ[3][3] = 1;

    // Rotation x
    matRotX[0][0] = 1;
    matRotX[0][1] = 0;
    matRotX[0][2] = 0;
    matRotX[0][3] = 0;
    matRotX[1][0] = 0;
    matRotX[1][1] = cosf(fTheta * 0.5f);
    matRotX[1][2] = sinf(fTheta * 0.5f);
    matRotX[1][3] = 0;
    matRotX[2][0] = 0;
    matRotX[2][1] = -sinf(fTheta * 0.5f);
    matRotX[2][2] = cosf(fTheta * 0.5f);
    matRotX[2][3] = 0;
    matRotX[3][0] = 0;
    matRotX[3][1] = 0;
    matRotX[3][2] = 0;
    matRotX[3][3] = 1;

    matScale[0][0] = meshScale;
    matScale[0][1] = 0;
    matScale[0][2] = 0;
    matScale[0][3] = 0;
    matScale[1][0] = 0;
    matScale[1][1] = meshScale;
    matScale[1][2] = 0;
    matScale[1][3] = 0;
    matScale[2][0] = 0;
    matScale[2][1] = 0;
    matScale[2][2] = meshScale;
    matScale[2][3] = 0;
    matScale[3][0] = 0;
    matScale[3][1] = 0;
    matScale[3][2] = 0;
    matScale[3][3] = meshScale;

    const float radius = 5.0f;
    float camX = sin(elapsed_time) * radius;
    float camZ = cos(elapsed_time) * radius;
    glm::mat4 view;


    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


//    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    for(auto id = 1; id <= entitySystem.getMeshes().size(); ++id) {
        std::vector<glm::mat3x3> mesh = entitySystem.getMeshes().at(id);

        meshScale= entitySystem.getScales().at(id);
        matScale = view * matScale ;

        //Draw triangles
        for (glm::mat3x4 tri: mesh) {
            glm::mat3x4 triTranslated;
//            triTranslated[0] = tri[0] + position4;
//            triTranslated[1] = tri[1] + position4;
//            triTranslated[2] = tri[2] + position4;

            glm::mat3x3 triProjected, triTranslated, triScale, triRotatedZ, triRotatedZX;


            MatrixVectorMultiplier(tri[0], triScale[0], matScale);
            MatrixVectorMultiplier(tri[1], triScale[1], matScale);
            MatrixVectorMultiplier(tri[2], triScale[2], matScale);

            triTranslated[0][2] = tri[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[1][2] = tri[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[2][2] = tri[2][2] + 3.0f + cameraSystem.getCameraPosition().z;
            // Compute translated triangle (in game world coordinates)
            triTranslated = triScale;
            triTranslated[0][2] = triScale[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[1][2] = triScale[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[2][2] = triScale[2][2] + 3.0f + cameraSystem.getCameraPosition().z;

//            MatrixVectorMultiplier(triScale[0], triRotatedZ[0], matRotZ);
//            MatrixVectorMultiplier(triScale[1], triRotatedZ[1], matRotZ);
//            MatrixVectorMultiplier(triScale[2], triRotatedZ[2], matRotZ);
//
//            MatrixVectorMultiplier(triRotatedZ[0], triRotatedZX[0], matRotX);
//            MatrixVectorMultiplier(triRotatedZ[1], triRotatedZX[1], matRotX);
//            MatrixVectorMultiplier(triRotatedZ[2], triRotatedZX[2], matRotX);
//
//            // Compute translated triangle (in game world coordinates)
//            triTranslated = triRotatedZX;
//            triTranslated[0][2] = triRotatedZX[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
//            triTranslated[1][2] = triRotatedZX[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
//            triTranslated[2][2] = triRotatedZX[2][2] + 3.0f + cameraSystem.getCameraPosition().z;

            glm::mat3x4 triProjected = matProj * triTranslated;

            glm::mat3x4 triProjected1;
            MatrixVectorMultiplier(triTranslated[0], triProjected1[0], matProj);
            MatrixVectorMultiplier(triTranslated[1], triProjected1[1], matProj);
            MatrixVectorMultiplier(triTranslated[2], triProjected1[2], matProj);

            bool a = triProjected1 == triProjected;


//            std::cout << glm::to_string(triTranslated) << std::endl;
//            std::cout << glm::to_string(matProj) << std::endl;
//            std::cout << glm::to_string(triProjected) << std::endl;
//            std::cout << glm::to_string(triProjected1) << std::endl;

//            // Compute translated triangle (in game world coordinates)
//            triTranslated = tri;
//            triTranslated[0][2] = tri[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
//            triTranslated[1][2] = tri[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
//            triTranslated[2][2] = tri[2][2] + 3.0f + cameraSystem.getCameraPosition().z;
//
//            MatrixVectorMultiplier(triTranslated[0], triProjected[0], matProj);
//            MatrixVectorMultiplier(triTranslated[1], triProjected[1], matProj);
//            MatrixVectorMultiplier(triTranslated[2], triProjected[2], matProj);

            //scale into view
            triProjected[0][0] += cameraSystem.getCameraPosition().x + 1.0f;
            triProjected[0][1] += cameraSystem.getCameraPosition().y + 1.0f;
            triProjected[1][0] += cameraSystem.getCameraPosition().x + 1.0f;
            triProjected[1][1] += cameraSystem.getCameraPosition().y + 1.0f;
            triProjected[2][0] += cameraSystem.getCameraPosition().x + 1.0f;
            triProjected[2][1] += cameraSystem.getCameraPosition().y + 1.0f;

            triProjected[0][0] *= 0.5f * (float) screenWidth;
            triProjected[0][1] *= 0.5f * (float) screenHeight;
            triProjected[1][0] *= 0.5f * (float) screenWidth;
            triProjected[1][1] *= 0.5f * (float) screenHeight;
            triProjected[2][0] *= 0.5f * (float) screenWidth;
            triProjected[2][1] *= 0.5f * (float) screenHeight;

            DrawTriangle(image, triProjected[0][0], triProjected[0][1],
                         triProjected[1][0], triProjected[1][1],
                         triProjected[2][0], triProjected[2][1],
                         0x0000);
        }
//        std:: cout << std::endl;
//        std:: cout << std::endl;
//        std:: cout << std::endl;
    }
//    std:: cout << std::endl;
//    std:: cout << std::endl;
//    std:: cout << std::endl;
//    std:: cout << std::endl;

    return image;
}
