#ifndef PSR_3D_RENDERER_LINEARALGEBRAMATH_H
#define PSR_3D_RENDERER_LINEARALGEBRAMATH_H

#include "glm/glm.hpp"

class LinearAlgebraMath {
public:
    glm::mat3x4 subtractVecFromMatrixColumnwise(glm::mat3x4 mat, glm::vec4 vec);

    /**
     * Returns a matrix which rotates vec1 to align with vec2 (length is not guaranteed).
     * TODO: Is new length is always the square of the initial length?
     */
    glm::mat3x3 rotateVec1OntoVec2(glm::vec3 vec1, glm::vec3 vec2);
};


#endif //PSR_3D_RENDERER_LINEARALGEBRAMATH_H
