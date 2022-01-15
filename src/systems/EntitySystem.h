#ifndef PSR_3D_RENDERER_ENTITYSYSTEM_H
#define PSR_3D_RENDERER_ENTITYSYSTEM_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <opencv2/opencv.hpp>
#include "../entities/EntityComponents.h"
#include "../systems/CameraSystem.h"
#include "../external-libraries/stl_reader.h"

class EntitySystem {
private:
    CameraSystem &cameraSystem;  // TODO: Should & be on first or second? Why does & make sense here?
    Positions positions;
    Orientations orientations;
    Scales scales;
    Meshes meshes;
    Colors colors;
    int64_t totalEntitiesAdded;

    std::vector<glm::mat3x4> MeshGenerator(std::string shape);

public:
    enum Entity {
        SPHERE, CUBE, TORUS, PYRAMID, AXES, GRID
    };

    explicit EntitySystem(CameraSystem &cameraSystem1);

    void updateGame(int keycode);

    EntityID getNewEntityID();

    void addEntity(Entity entity, Position position);

    void removeEntity(EntityID id);

    void addPositionWithID(Position position, EntityID id);
    Position getPositionWithID(EntityID id);

    void addOrientationWithID(Orientation orientation, EntityID id);
    Orientation getOrientationWithID(EntityID id);

    void addScaleWithID(Scale scale, EntityID id);
    Scale getScaleWithID(EntityID id);

    void addMeshWithID(Mesh mesh, EntityID id);
    Mesh getMeshWithID(EntityID id);

    void addColorWithID(Color color, EntityID id);
    Color getColorWithID(EntityID id);

    Meshes::const_iterator getMeshesIterBegin();

    Meshes::const_iterator getMeshesIterEnd();
};


#endif //PSR_3D_RENDERER_ENTITYSYSTEM_H
