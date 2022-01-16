#include "EntitySystem.h"
#include "../external-libraries/stl_reader.h"

EntitySystem::EntitySystem(CameraSystem &cameraSystem1)
        : cameraSystem(cameraSystem1) {}

void EntitySystem::addEntity(Entity entity, glm::vec3 position) {
    // Obtain new entity ID
    totalEntitiesAdded++;
    EntityID id = totalEntitiesAdded;

    // TODO: Fill this with real code. Also, use builders?
    switch (entity) {
        case SPHERE:
            std::cout << "SPHERE" << std::endl;
            positions[id] = position;
            orientations[id] = -cameraSystem.getCameraOrientation();
            scales[id] = glm::vec3(1);
            colors[id] = {0, 0, 255};
            meshes[id] = MeshGenerator("sphere");
            std::cout << "Finished." << std::endl;
            break;
        case CUBE:
            std::cout << "CUBE" << std::endl;
            positions[id] = position;
            orientations[id] = -cameraSystem.getCameraOrientation();
            scales[id] = glm::vec3(1);
            colors[id] = {255, 0, 0};
            meshes[id] = MeshGenerator("cube");
            std::cout << "Finished." << std::endl;
            break;
        case TORUS:
            std::cout << "TORUS" << std::endl;
            positions[id] = position;
            orientations[id] = -cameraSystem.getCameraOrientation();
            scales[id] = glm::vec3(1);
            colors[id] = {0, 255, 0};
            meshes[id] = MeshGenerator("torus");
            std::cout << "Finished." << std::endl;
            break;
        case PYRAMID:
            std::cout << "PYRAMID" << std::endl;
            positions[id] = position;
            orientations[id] = -cameraSystem.getCameraOrientation();
            scales[id] = glm::vec3(1);
            colors[id] = {255, 0, 255};
            meshes[id] = MeshGenerator("pyramid");
            std::cout << "Finished." << std::endl;
            break;
        case AXES:
            std::cout << "AXES" << std::endl;
            positions[id] = position;
            orientations[id] = glm::vec3(1, 0, 0);
            scales[id] = glm::vec3(1);
            colors[id] = {0, 0, 0};
            meshes[id] = MeshGenerator("axes");
            std::cout << "Finished." << std::endl;
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

Scales EntitySystem::getScales() {
    return scales;
}

Colors EntitySystem::getColors() {
    return colors ;
}

void EntitySystem::updateGame(int keycode) {
    switch (keycode) {
        case 83: // 'S' - SPHERE
            addEntity(SPHERE, cameraSystem.getCameraPosition());
            break;
        case 67: // 'C' - CUBE
            addEntity(CUBE, cameraSystem.getCameraPosition());
            break;
        case 84: // 'T' - TORUS
            addEntity(TORUS, cameraSystem.getCameraPosition());
            break;
        case 80: // 'P' - PYRAMID
            addEntity(PYRAMID, cameraSystem.getCameraPosition());
            break;
        case 65: // 'A' - AXES
            addEntity(AXES, glm::vec3(0));
            break;
    }
}

std::vector<glm::mat3x4> EntitySystem::MeshGenerator(std::string shape) {
    std::vector<float> coords, normals;
    std::vector<unsigned int> tris, solids;
    //TODO: Need to figure a way to make relative paths work
    std::string sh = "./meshes/" + shape + ".stl"; // This line is not working yet
    try {
        stl_reader::ReadStlFile(sh.c_str(), coords, normals, tris, solids);
        const size_t numTris = tris.size() / 3;
        std::vector<glm::mat3x4> mesh;

        for (size_t itri = 0; itri < numTris; ++itri) {
            std::vector<glm::vec4> f;
            for (size_t icorner = 0; icorner < 3; ++icorner) {
                float *c = &coords[3 * tris[3 * itri + icorner]];
                glm::vec4 a = glm::vec4(c[0], c[1], c[2], 1);
                f.push_back(a);
            }
            glm::mat3x4 triMatrix = glm::mat3x4(f[0], f[1], f[2]);
            mesh.push_back(triMatrix);
        }
        return mesh;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
