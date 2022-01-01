#include "EntitySystem.h"

EntitySystem::EntitySystem() {
    positions = Positions();
    orientations = Orientations();
    meshes = Meshes();
    numEntities = 0;
}

void EntitySystem::addEntity(Entity entity) {
    // Obtain new entity ID
    numEntities++;
    EntityID id = numEntities;

    // TODO: Fill this with real code. Also, use builders?
    switch(entity)
    {
        case SPHERE:
            std::cout << "SPHERE" << std::endl;
            positions[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            orientations[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            meshes[id] = cv::Mat();
            break;
        case CUBE:
            std::cout << "CUBE" << std::endl;
            positions[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            orientations[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            meshes[id] = cv::Mat();
            break;
        case TORUS:
            std::cout << "TORUS" << std::endl;
            positions[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            orientations[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            meshes[id] = cv::Mat();
            break;
        case PYRAMID:
            std::cout << "PYRAMID" << std::endl;
            positions[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            orientations[id] = glm::vec3{0.0f, 0.0f, 0.0f};
            meshes[id] = cv::Mat();
            break;
    }
}

void EntitySystem::removeEntity(EntityID id) {
    positions.erase(id);
    orientations.erase(id);
    meshes.erase(id);
}

Positions EntitySystem::getPositions() {
    return positions;
}

Orientations EntitySystem::getOrientations() {
    return orientations;
}

Meshes EntitySystem::getMeshes() {
    return meshes;
}

void EntitySystem::updateGame(int keycode) {
    switch(keycode)
    {
        case 83: // 'S' - SPHERE
            addEntity(SPHERE);
            break;
        case 67: // 'C' - CUBE
            addEntity(CUBE);
            break;
        case 84: // 'T' - TORUS
            addEntity(TORUS);
            break;
        case 80: // 'P' - PYRAMID
            addEntity(PYRAMID);
            break;
    }
}
