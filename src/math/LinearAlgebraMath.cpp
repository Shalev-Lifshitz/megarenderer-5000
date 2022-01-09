#include "LinearAlgebraMath.h"

glm::mat3x4 LinearAlgebraMath::subtractVecFromMatrixColumnwise(glm::mat3x4 mat, glm::vec4 vec) {
    mat[0] = mat[0] - vec;
    mat[1] = mat[1] - vec;
    mat[2] = mat[2] - vec;
    return mat;
}

glm::mat3x3 LinearAlgebraMath::rotateVec1OntoVec2(glm::vec3 vec1, glm::vec3 vec2) {
    if (vec1 == vec2) {
        return glm::mat3x3(1);
    }

    // https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
    glm::vec3 v = glm::cross(vec1, vec2);
    float s = glm::length(v);
    float c = glm::dot(vec1, vec2);

    auto vSkewMatrix = glm::mat3x3(0);
    vSkewMatrix[1][0] = -v.z;
    vSkewMatrix[0][1] = v.z;
    vSkewMatrix[2][0] = v.y;
    vSkewMatrix[0][2] = -v.y;
    vSkewMatrix[2][1] = -v.x;
    vSkewMatrix[1][2] = v.x;

    glm::mat3x3 rotationMatrix = glm::mat3x3(1) + vSkewMatrix + (vSkewMatrix * vSkewMatrix * ((1 - c) / (s * s)));
    return rotationMatrix;
}
