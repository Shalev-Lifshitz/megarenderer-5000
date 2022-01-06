#ifndef PSR_3D_RENDERER_ENTITYCOMPONENTS_H
#define PSR_3D_RENDERER_ENTITYCOMPONENTS_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <opencv2/opencv.hpp>

// Define an EntityID type
using EntityID = int64_t;

// Define the type of each component map
template<typename Type>
using ComponentMap = std::unordered_map<EntityID, Type>;
using Positions = ComponentMap<glm::vec3>;
using Orientations = ComponentMap<glm::vec3>;
using Scales = ComponentMap<float>; // TODO: make this a vector
using Meshes = ComponentMap<std::vector<glm::mat3x4>>; // Each vector has shape numTriangles x 3 x 4

#endif //PSR_3D_RENDERER_ENTITYCOMPONENTS_H
