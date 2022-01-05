#ifndef PSR_3D_RENDERER_RENDERSYSTEM_H
#define PSR_3D_RENDERER_RENDERSYSTEM_H


#include "CameraSystem.h"
#include "EntitySystem.h"
#include <memory.h>

class RenderSystem {
private:
    EntitySystem entitySystem;
    CameraSystem cameraSystem;
public:
    RenderSystem(CameraSystem &cameraSystem, EntitySystem &entitySystem);

    std::unique_ptr<cv::Mat> renderScene(cv::Mat& imageBackground);

    struct triangle
    {
        glm::vec3 p[3];
    };

    struct mesh
    {
        std::vector<triangle> tris;
    };

    std::unique_ptr<cv::Mat> renderCube(cv::Mat &imageBackground);

private:
    mesh meshCube;
    glm::mat4x4 matProj;

    void MatrixVectorMultiplyer(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &m)
    {
        o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + m[3][0];
        o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + m[3][1];
        o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + m[3][2];
        float w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + m[3][3];

        if (w != 0.0f)
        {
            o.x/= w; o.y/= w; o.z/= w;
        }
    }

};


#endif //PSR_3D_RENDERER_RENDERSYSTEM_H
