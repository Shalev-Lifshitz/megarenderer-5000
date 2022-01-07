#ifndef PSR_3D_RENDERER_GAME_H
#define PSR_3D_RENDERER_GAME_H

#include <string>
#include "../systems/CameraSystem.h"
#include "../systems/EntitySystem.h"
#include "../systems/RenderSystem.h"


class Game {
private:
    CameraSystem &cameraSystem;
    EntitySystem &entitySystem;
    RenderSystem &renderSystem;
public:
    explicit Game(
            CameraSystem &cameraSystem1,
            EntitySystem &entitySystem1,
            RenderSystem &renderSystem1);

    bool runGameLoop(std::string backgroundImagePath, int screenHeight, int screenWidth);
};


#endif //PSR_3D_RENDERER_GAME_H
