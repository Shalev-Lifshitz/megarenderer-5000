#include "RenderSystem.h"
#include <tuple>
#include <utility>
#include <type_traits>

void RenderSystem::MatrixVectorMultiplier(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &m) {
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

std::unique_ptr<cv::Mat> RenderSystem::renderScene(cv::Mat& imageBackground) {
    std::unique_ptr<cv::Mat> image = std::make_unique<cv::Mat>(imageBackground.clone());

    auto screenHeight = 800;
    auto screenWidth = 800;

    for(auto const &pair: entitySystem.getMeshes()) {
        std::vector<glm::mat3x3> mesh = pair.second;

        //Projection Matrix
        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float) screenHeight / (float) screenWidth;
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

        matProj[0][0] = fAspectRatio * fFovRad;
        matProj[0][1] = 0.0f;
        matProj[1][1] = fFovRad;
        matProj[1][0] = 0.0f;
        matProj[2][1] = 0.0f;
        matProj[2][2] = fFar / (fFar - fNear);
        matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProj[2][3] = 1.0f;
        matProj[3][3] = 0.0f;

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
        matRotX[2][2] = cosf(fTheta * 0.5f);
        matRotX[3][3] = 1;


        //Draw triangles
        for (glm::mat3x3 tri: mesh) {
            glm::mat3x3 triProjected, triTranslated;

            // Compute translated triangle (in game world coordinates)
            triTranslated = tri;
            triTranslated[0][2] = tri[0][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[1][2] = tri[1][2] + 3.0f + cameraSystem.getCameraPosition().z;
            triTranslated[2][2] = tri[2][2] + 3.0f + cameraSystem.getCameraPosition().z;

            MatrixVectorMultiplier(triTranslated[0], triProjected[0], matProj);
            MatrixVectorMultiplier(triTranslated[1], triProjected[1], matProj);
            MatrixVectorMultiplier(triTranslated[2], triProjected[2], matProj);

            //scale into view
            triProjected[0][0] += cameraSystem.getCameraPosition().x;
            triProjected[0][1] += cameraSystem.getCameraPosition().x;
            triProjected[1][0] += cameraSystem.getCameraPosition().x;
            triProjected[1][1] += cameraSystem.getCameraPosition().x;
            triProjected[2][0] += cameraSystem.getCameraPosition().x;
            triProjected[2][1] += cameraSystem.getCameraPosition().x;

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
    }
    return image;
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