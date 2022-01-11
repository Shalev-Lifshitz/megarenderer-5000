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
    glm::vec2 mousePosition;
public:
    explicit Game(
            CameraSystem &cameraSystem1,
            EntitySystem &entitySystem1,
            RenderSystem &renderSystem1);

    bool runGameLoop(std::string backgroundImagePath, int screenHeight, int screenWidth);

    void recordMouseEvent(int event, int x, int y, int flags);
};


#endif //PSR_3D_RENDERER_GAME_H
