#include "RenderSystem.h"
#include <tuple>
#include <utility>
#include <type_traits>

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

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat& imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

//    for(int i = 0; (i = entitySystem.getMeshes().size()); i++) {
//
//        entitySystem.getMeshes().at(i);
//    }
    //TODO
    return renderCube(imageBackground);
}

std::unique_ptr<cv::Mat> RenderSystem::renderCube(cv::Mat& imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

    auto screenHeight = image->rows;
    auto screenWidth = image->cols;

    meshCube.tris = {

            //South
            {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)},
            {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},

            //East
            {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
            {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f)},

            //North
            {glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},
            {glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},

            //West
            {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

            //Top
            {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
            {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},

            //Bottom
            {glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
            {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    };

    //Projection Matrix
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float) screenHeight / (float) screenWidth;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj[0][0] = fAspectRatio * fFovRad;
    matProj[1][1] = fFovRad;
    matProj[2][2] = fFar / (fFar - fNear);
    matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj[2][3] = 1.0f;
    matProj[3][3] = 0.0f;

    for (auto tri: meshCube.tris) {
        glm::mat4x4 matRotZ, matRotX;
        auto fTheta = 1.0f * 150.5f;

        // Rotation z
        matRotZ[0][0] = cosf(fTheta);
        matRotZ[0][1] = sinf(fTheta);
        matRotZ[1][1] = -sinf(fTheta);
        matRotZ[2][1] = cosf(fTheta);
        matRotZ[2][2] = 1;
        matRotZ[3][3] = 1;

        // Rotation x
        matRotX[0][0] = 1;
        matRotX[1][1] = cosf(fTheta * 0.5f);
        matRotX[1][2] = sinf(fTheta * 0.5f);
        matRotX[2][1] = -sinf(fTheta * 0.5f);
        matRotX[2][2] = cosf(fTheta * 0.5f);;
        matRotX[3][3] = 1;


        //Draw triangles
        for (auto tri: meshCube.tris) {
            triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

            MatrixVectorMultiplyer(tri.p[0], triRotatedZ.p[0], matRotZ);
            MatrixVectorMultiplyer(tri.p[1], triRotatedZ.p[1], matRotZ);
            MatrixVectorMultiplyer(tri.p[2], triRotatedZ.p[2], matRotZ);

            MatrixVectorMultiplyer(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
            MatrixVectorMultiplyer(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
            MatrixVectorMultiplyer(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

//            auto const transform = matRotZ * matRotX;

            triTranslated = triRotatedZX;
            triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
            triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
            triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

            MatrixVectorMultiplyer(triTranslated.p[0], triProjected.p[0], matProj);
            MatrixVectorMultiplyer(triTranslated.p[1], triProjected.p[1], matProj);
            MatrixVectorMultiplyer(triTranslated.p[2], triProjected.p[2], matProj);

            //scale into view
            triProjected.p[0].x += 1.0f;
            triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f;
            triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f;
            triProjected.p[2].y += 1.0f;

            triProjected.p[0].x *= 0.5f * (float) screenWidth;
            triProjected.p[0].y *= 0.5f * (float) screenHeight;
            triProjected.p[1].x *= 0.5f * (float) screenWidth;
            triProjected.p[1].y *= 0.5f * (float) screenHeight;
            triProjected.p[2].x *= 0.5f * (float) screenWidth;
            triProjected.p[2].y *= 0.5f * (float) screenHeight;

            DrawTriangle(image, triProjected.p[0].x, triProjected.p[0].y,
                         triProjected.p[1].x, triProjected.p[1].y,
                         triProjected.p[2].x, triProjected.p[2].y,
                         0x0000);
        }
        return image;
    }
}












//template<typename P>
//void triangle_rasterizer(const P* p0, const P* p1, const P* p2){
//    //Does the rasterization starting from top and going to the bottom
//
//    requires std::invoke_result
//
//    auto [x0, y0, x1, y1, x2, y2] = std::tuple_cat(getXYcoords(*p0), getXYcoords(*p1), getXYcoords(*p2));
//
//    // We sort the points by their Y-coordinates, from top to bottom, and order by X-coordinates when Y is equal
//    if(std::tie(y1, x1) < std::tie(y0, x0)) {
//        std::swap(x0, x1);
//        std::swap(y0, y1);
//        std::swap(p0, p1);}
//    if(std::tie(y2, x2) < std::tie(y0, x0)) {
//        std::swap(x0, x2);
//        std::swap(y0, y2);
//        std::swap(p0, p2);}
//    if(std::tie(y2, x2) < std::tie(y0, x0)) {
//        std::swap(x1, x2);
//        std::swap(y1, y2);
//        std::swap(p1, p2);}
//
//    if(y2 == y0){
//        return;
//    }
//
//    bool short_side = (x2 - x0) * (y1 - y0) < (x1 - x0) * (y2 - y0);