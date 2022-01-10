#include "LinearAlgebraMath.h"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

glm::mat3x4 LinearAlgebraMath::subtractVecFromMatrixColumnwise(glm::mat3x4 mat, glm::vec4 vec) {
    mat[0] = mat[0] - vec;
    mat[1] = mat[1] - vec;
    mat[2] = mat[2] - vec;
    return mat;
}

glm::mat3x3 LinearAlgebraMath::getMatrixToRotateAtoB(glm::vec3 a, glm::vec3 b) {
    if (LinearAlgebraMath::equalsEstimate(a, b)) {
        return glm::mat3x3(1);
    }

    // Create Quaternion to represent the rotation
    auto v = glm::cross(a, b);
    double aLengthSquared = pow(glm::length(a), 2);
    double bLengthSquared = pow(glm::length(b), 2);
    double w = sqrt(aLengthSquared * bLengthSquared) + glm::dot(a, b);
    glm::quat quaternion = glm::normalize(glm::quat(w, v.x, v.y, v.z));

    // Compute rotation matrix from rotation quaternion
    glm::mat3x3 rotationMatrix = glm::toMat3(quaternion);
    return rotationMatrix;
}

bool LinearAlgebraMath::equalsEstimate(glm::vec3 a, glm::vec3 b) {
    auto elem0IsEqual = std::abs(a[0] - b[0]) < epsilon;
    auto elem1IsEqual = std::abs(a[1] - b[1]) < epsilon;
    auto elem2IsEqual = std::abs(a[2] - b[2]) < epsilon;
    return elem0IsEqual and elem1IsEqual and elem2IsEqual;
}

bool LinearAlgebraMath::equalsEstimate(glm::vec4 a, glm::vec4 b) {
    auto elem0IsEqual = std::abs(a[0] - b[0]) < epsilon;
    auto elem1IsEqual = std::abs(a[1] - b[1]) < epsilon;
    auto elem2IsEqual = std::abs(a[2] - b[2]) < epsilon;
    auto elem3IsEqual = std::abs(a[3] - b[3]) < epsilon;
    return elem0IsEqual and elem1IsEqual and elem2IsEqual and elem3IsEqual;
}

bool LinearAlgebraMath::equalsEstimate(glm::quat a, glm::quat b) {
    auto elem0IsEqual = std::abs(a[0] - b[0]) < epsilon;
    auto elem1IsEqual = std::abs(a[1] - b[1]) < epsilon;
    auto elem2IsEqual = std::abs(a[2] - b[2]) < epsilon;
    auto elem3IsEqual = std::abs(a[3] - b[3]) < epsilon;
    return elem0IsEqual and elem1IsEqual and elem2IsEqual and elem3IsEqual;
}

bool LinearAlgebraMath::equalsEstimate(glm::mat3x3 a, glm::mat3x3 b) {
    auto col0IsEqual = equalsEstimate(a[0],b[0]);
    auto col1IsEqual = equalsEstimate(a[1],b[1]);
    auto col2IsEqual = equalsEstimate(a[2],b[2]);
    return col0IsEqual and col1IsEqual and col2IsEqual;
}

bool LinearAlgebraMath::equalsEstimate(glm::mat3x4 a, glm::mat3x4 b) {
    auto col0IsEqual = equalsEstimate(a[0],b[0]);
    auto col1IsEqual = equalsEstimate(a[1],b[1]);
    auto col2IsEqual = equalsEstimate(a[2],b[2]);
    return col0IsEqual and col1IsEqual and col2IsEqual;
}

bool LinearAlgebraMath::equalsEstimate(glm::mat4x4 a, glm::mat4x4 b) {
    auto col0IsEqual = equalsEstimate(a[0],b[0]);
    auto col1IsEqual = equalsEstimate(a[1],b[1]);
    auto col2IsEqual = equalsEstimate(a[2],b[2]);
    auto col3IsEqual = equalsEstimate(a[3],b[3]);
    return col0IsEqual and col1IsEqual and col2IsEqual and col3IsEqual;
}

