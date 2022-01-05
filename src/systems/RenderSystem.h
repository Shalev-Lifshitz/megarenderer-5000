#ifndef PSR_3D_RENDERER_RENDERSYSTEM_H
#define PSR_3D_RENDERER_RENDERSYSTEM_H


#include "CameraSystem.h"
#include "EntitySystem.h"
#include <memory.h>

class RenderSystem {
private:
    EntitySystem& entitySystem;
    CameraSystem& cameraSystem;
public:
    RenderSystem(EntitySystem& entitySystem1, CameraSystem& cameraSystem1);

    std::unique_ptr<cv::Mat> renderScene(cv::Mat& imageBackground);
private:
    glm::mat4x4 matProj;

    void MatrixVectorMultiplier(glm::vec4 &i, glm::vec4 &o, glm::mat4x4 &m);
};

#endif //PSR_3D_RENDERER_RENDERSYSTEM_H
