#ifndef PSR_3D_RENDERER_GAME_H
#define PSR_3D_RENDERER_GAME_H

#include <string>
#include "../systems/CameraSystem.h"
#include "../systems/EntitySystem.h"
#include "../systems/RenderSystem.h"


class Game {
private:
    CameraSystem cameraSystem;
    EntitySystem entitySystem;
    RenderSystem renderSystem;
public:
    explicit Game(CameraSystem cameraSystem, EntitySystem entitySystem, RenderSystem renderSystem);
    bool runGameLoop(std::string backgroundImagePath);
};


#endif //PSR_3D_RENDERER_GAME_H
