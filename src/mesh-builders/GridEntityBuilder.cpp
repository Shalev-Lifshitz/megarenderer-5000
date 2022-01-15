#include "GridEntityBuilder.h"

// TODO: Create a new component for meshes made up of lines and not triangles.
void GridEntityBuilder::buildEntity(EntitySystem &entitySystem, glm::vec3 position,
                                    glm::vec3 orientation, glm::vec3 scale) {
    int boxSize = 40;
    buildXLines(entitySystem, boxSize, position, orientation, scale);
    buildYLines(entitySystem, boxSize, position, orientation, scale);
    buildZLines(entitySystem, boxSize, position, orientation, scale);
}

void GridEntityBuilder::buildXLines(EntitySystem &entitySystem, int boxSize,
                                    glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) {
    std::vector<glm::mat3x4> xLinesMesh;
    for (int i = -boxSize; i <= boxSize; i++) {
        glm::mat3x4 xLineFloor = genLineX(boxSize, glm::vec3(0, 0, i));
        glm::mat3x4 xLineCeiling = genLineX(boxSize, glm::vec3(0, boxSize, i));
        glm::mat3x4 xLineWall1 = genLineX(boxSize, glm::vec3(0, i, -boxSize));
        glm::mat3x4 xLineWall2 = genLineX(boxSize, glm::vec3(0, i, boxSize));
        xLinesMesh.push_back(xLineFloor);
        xLinesMesh.push_back(xLineCeiling);
        xLinesMesh.push_back(xLineWall1);
        xLinesMesh.push_back(xLineWall2);
    }

    Meshes *meshes = entitySystem.getMeshes();
    Positions *positions = entitySystem.getPositions();
    Orientations *orientations = entitySystem.getOrientations();
    Colors *colors = entitySystem.getColors();
    Scales *scales = entitySystem.getScales();

    EntityID id = entitySystem.getNewEntityID();
    meshes[id] = xLinesMesh;
    positions[id] = position;
    orientations[id] = orientation;
    colors[id] = {255, 0, 0};
    scales[id] = scale;
}

void GridEntityBuilder::buildYLines(EntitySystem &entitySystem, int boxSize,
                                    glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) {
    std::vector<glm::mat3x4> yLinesMesh;
    for (int i = -boxSize; i <= boxSize; i++) {
        glm::mat3x4 yLineWall1 = genLineZ(boxSize, glm::vec3(i, 0, boxSize));
        glm::mat3x4 yLineWall2 = genLineZ(boxSize, glm::vec3(i, 0, -boxSize));
        glm::mat3x4 yLineWall3 = genLineZ(boxSize, glm::vec3(boxSize, 0, i));
        glm::mat3x4 yLineWall4 = genLineZ(boxSize, glm::vec3(-boxSize, 0, i));
        yLinesMesh.push_back(yLineWall4);
    }

    Meshes meshes = *entitySystem.getMeshes();
    Positions positions = *entitySystem.getPositions();
    Orientations orientations = *entitySystem.getOrientations();
    Colors colors = *entitySystem.getColors();
    Scales scales = *entitySystem.getScales();

    EntityID id = entitySystem.getNewEntityID();
    meshes[id] = yLinesMesh;
    positions[id] = position;
    orientations[id] = orientation;
    colors[id] = {0, 255, 0};
    scales[id] = scale;
}

void GridEntityBuilder::buildZLines(EntitySystem &entitySystem, int boxSize,
                                    glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) {
    std::vector<glm::mat3x4> zLinesMesh;
    for (int i = -boxSize; i <= boxSize; i++) {
        glm::mat3x4 zLineFloor = genLineZ(boxSize, glm::vec3(i, 0, 0));
        glm::mat3x4 zLineCeiling = genLineZ(boxSize, glm::vec3(i, boxSize, 0));
        glm::mat3x4 zLineWall1 = genLineZ(boxSize, glm::vec3(boxSize, i, 0));
        glm::mat3x4 zLineWall2 = genLineZ(boxSize, glm::vec3(-boxSize, i, 0));
        zLinesMesh.push_back(zLineFloor);
        zLinesMesh.push_back(zLineCeiling);
        zLinesMesh.push_back(zLineWall1);
        zLinesMesh.push_back(zLineWall2);
    }

    Meshes meshes = *entitySystem.getMeshes();
    Positions positions = *entitySystem.getPositions();
    Orientations orientations = *entitySystem.getOrientations();
    Colors colors = *entitySystem.getColors();
    Scales scales = *entitySystem.getScales();

    EntityID id = entitySystem.getNewEntityID();
    meshes[id] = zLinesMesh;
    positions[id] = position;
    orientations[id] = orientation;
    colors[id] = {0, 0, 255};
    scales[id] = scale;
}

glm::mat3x4 GridEntityBuilder::genLineX(float length, glm::vec3 center) {
    auto p1 = glm::vec4(center.x - length, center.y, center.z, 1);
    auto p2 = glm::vec4(center.x + length, center.y, center.z, 1);
    return {p1, p2, p2};
}

glm::mat3x4 GridEntityBuilder::genLineY(float length, glm::vec3 center) {
    auto p1 = glm::vec4(center.x, center.y - length, center.z, 1);
    auto p2 = glm::vec4(center.x, center.y + length, center.z, 1);
    return {p1, p2, p2};
}

glm::mat3x4 GridEntityBuilder::genLineZ(float length, glm::vec3 center) {
    auto p1 = glm::vec4(center.x, center.y, center.z - length, 1);
    auto p2 = glm::vec4(center.x, center.y, center.z + length, 1);
    return {p1, p2, p2};
}


