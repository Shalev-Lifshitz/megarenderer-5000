#ifndef PSR_3D_RENDERER_GRIDENTITYBUILDER_H
#define PSR_3D_RENDERER_GRIDENTITYBUILDER_H


#include "EntityBuilder.h"

class GridEntityBuilder : EntityBuilder {
private:
    glm::mat3x4 genLineX(float length, glm::vec3 center);

    glm::mat3x4 genLineY(float length, glm::vec3 center);

    glm::mat3x4 genLineZ(float length, glm::vec3 center);

    void buildXLines(EntitySystem &entitySystem, int boxSize,
                     glm::vec3 position, glm::vec3 orientation, glm::vec3 scale);

    void buildYLines(EntitySystem &entitySystem, int boxSize,
                     glm::vec3 position, glm::vec3 orientation, glm::vec3 scale);

    void buildZLines(EntitySystem &entitySystem, int boxSize,
                     glm::vec3 position, glm::vec3 orientation, glm::vec3 scale);

public:
    void buildEntity(EntitySystem &entitySystem, glm::vec3 position,
                     glm::vec3 orientation, glm::vec3 scale) override;
};


#endif //PSR_3D_RENDERER_GRIDENTITYBUILDER_H
