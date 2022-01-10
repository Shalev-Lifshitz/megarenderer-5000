#ifndef PSR_3D_RENDERER_LINEARALGEBRAMATH_H
#define PSR_3D_RENDERER_LINEARALGEBRAMATH_H

#include "glm/glm.hpp"

class LinearAlgebraMath {
public:
    constexpr static const float epsilon = 0.00001;

    static glm::mat3x4 subtractVecFromMatrixColumnwise(glm::mat3x4 mat, glm::vec4 vec);

    /**
     * Returns a matrix which rotates a to align with b (length is converved).
     */
    static glm::mat3x3 getMatrixToRotateAtoB(glm::vec3 a, glm::vec3 b);

    static bool equalsEstimate(glm::vec3 a, glm::vec3 b);

    static bool equalsEstimate(glm::vec4 a, glm::vec4 b);

    static bool equalsEstimate(glm::quat a, glm::quat b);

    static bool equalsEstimate(glm::mat3x3 a, glm::mat3x3 b);

    static bool equalsEstimate(glm::mat3x4 a, glm::mat3x4 b);

    static bool equalsEstimate(glm::mat4x4 a, glm::mat4x4 b);
};


#endif //PSR_3D_RENDERER_LINEARALGEBRAMATH_H
