#include <vector>
#include "TestLinearAlgebraMath.h"
#include "LinearAlgebraMath.h"
#include "glm/gtx/string_cast.hpp"
#include "iostream"

void TestLinearAlgebraMath::testSubtractVecFromMatrixColumnwise() {
    LinearAlgebraMath math;

    auto mat = glm::mat3x4(1);
    auto vec = glm::vec4(0, 1, 0, 0);

    auto expected = glm::mat3x4(1);
    expected[0][1] = -1;
    expected[1][1] = 0;
    expected[2][1] = -1;

    auto result = math.subtractVecFromMatrixColumnwise(mat, vec);

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

    auto epsilon = 0.00001;
    for (glm::vec3 v1: vecs) {
        for (glm::vec3 v2: vecs) {
            auto rotationMatrix = math.rotateVec1OntoVec2(v1, v2);
            auto v1Transformed = rotationMatrix * v1;

//            std::cout << std::endl;
//            std::cout << "v1: "<< glm::to_string(v1) << std::endl;
//            std::cout << "Vec1Length: " << glm::length(v1) << std::endl;
//            std::cout << "v2: " << glm::to_string(v2) << std::endl;
//            std::cout << "rotationMatrix: " << glm::to_string(rotationMatrix) << std::endl;
//            std::cout << "v1Transformed: " << glm::to_string(v1Transformed) << std::endl;

            float diff = std::abs(glm::length(glm::normalize(v1Transformed) - glm::normalize(v2)));
            assert(diff < epsilon);

            // Verify that the matrix conserves vector length
            for (glm::vec3 v3: vecs) {
                auto v3Length = glm::length(v3);

                auto v3Transformed = rotationMatrix * v3;
                auto v3TransformedLength = glm::length(v3Transformed);

                float diffLength = std::abs(v3TransformedLength - v3Length);
                assert(diffLength < epsilon);
            }
        }
    }
}

void TestLinearAlgebraMath::testAll() {
    testSubtractVecFromMatrixColumnwise();
    testRotateVec1OntoVec2();
}
