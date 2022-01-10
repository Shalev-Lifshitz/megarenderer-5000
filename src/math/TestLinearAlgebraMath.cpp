#include <vector>
#include "TestLinearAlgebraMath.h"
#include "LinearAlgebraMath.h"
#include "glm/gtx/string_cast.hpp"
#include "iostream"
#include "glm/gtx/vector_angle.hpp"

void TestLinearAlgebraMath::testSubtractVecFromMatrixColumnwise() {
    auto mat = glm::mat3x4(1);
    auto vec = glm::vec4(0, 1, 0, 0);

    auto expected = glm::mat3x4(1);
    expected[0][1] = -1;
    expected[1][1] = 0;
    expected[2][1] = -1;

    auto result = LinearAlgebraMath::subtractVecFromMatrixColumnwise(mat, vec);

    assert(result == expected);
}

void TestLinearAlgebraMath::testRotateVec1OntoVec2() {
    LinearAlgebraMath math;

    std::vector<glm::vec3> vecs = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(14, 28, 0),
            glm::vec3(78, 55, 22),
            glm::vec3(2, 1, 1),
            glm::vec3(121, 156, 6888),
    };

    // TODO: Count num sigfigs that are equal instead of after decimal points (lengthEpsilon)
    //  since large numbers can be less accurate after decimal point (but may have
    //  same number of equal sigfigs).
    float lengthEpsilon = 0.01;
    for (glm::vec3 v1: vecs) {
        for (glm::vec3 v2: vecs) {
            auto rotationMatrix = LinearAlgebraMath::getMatrixToRotateAtoB(v1, v2);
            auto v1Rotated = rotationMatrix * v1;

            auto v1RotatedNormalized = glm::normalize(v1Rotated);
            auto v2Normalized = glm::normalize(v2);

            bool printDebugInfo = false;
            if (printDebugInfo) {
                std::cout << std::endl;
                std::cout << "rotationMatrix: " << glm::to_string(rotationMatrix) << std::endl;
                std::cout << "v1: "<< glm::to_string(v1) << std::endl;
                std::cout << "v1 length: " << glm::length(v1) << std::endl;

                std::cout << "v2: " << glm::to_string(v2) << std::endl;
                std::cout << "v2 length: " << glm::length(v2) << std::endl;
                std::cout << "v2 normalized: " << glm::to_string(v2Normalized) << std::endl;

                std::cout << "v1Rotated: " << glm::to_string(v1Rotated) << std::endl;
                std::cout << "v1Rotated length: " << glm::length(v1Rotated) << std::endl;
                std::cout << "v1Rotated normalized: " << glm::to_string(v1RotatedNormalized) << std::endl;
            }

            // We compare normalized versions of v1Rotated and v2 since they do not have to be the same length
            assert(LinearAlgebraMath::equalsEstimate(v1RotatedNormalized, v2Normalized));
            // Verify that the matrix conserves vector length
            assert(abs(glm::length(v1) - glm::length(v1Rotated)) < lengthEpsilon);

            // Verify that the matrix conserves the length of other vectors
            for (glm::vec3 v3: vecs) {
                auto v3Transformed = rotationMatrix * v3;

                float v3Length = glm::length(v3);
                float v3TransformedLength = glm::length(v3Transformed);

                float diffLength = std::abs(v3TransformedLength - v3Length);
                assert(diffLength < lengthEpsilon);
            }
        }
    }
}

void TestLinearAlgebraMath::testAll() {
    testSubtractVecFromMatrixColumnwise();
    testRotateVec1OntoVec2();
}
