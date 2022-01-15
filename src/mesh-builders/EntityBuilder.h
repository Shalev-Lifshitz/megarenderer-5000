#ifndef PSR_3D_RENDERER_ENTITYBUILDER_H
#define PSR_3D_RENDERER_ENTITYBUILDER_H

#include <vector>
#include "glm/glm.hpp"
#include "../systems/EntitySystem.h"

class EntityBuilder {
    /**
     * Create entity and add relevant components to entitySystem.
     * @param entitySystem The entitySystem which will be mutated to contain the new entity's data.
     */
    virtual void buildEntity(EntitySystem &entitySystem, glm::vec3 position,
                             glm::vec3 orientation, glm::vec3 scale) = 0;
};


#endif //PSR_3D_RENDERER_ENTITYBUILDER_H
