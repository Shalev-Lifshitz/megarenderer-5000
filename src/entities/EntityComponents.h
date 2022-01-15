#ifndef PSR_3D_RENDERER_ENTITYCOMPONENTS_H
#define PSR_3D_RENDERER_ENTITYCOMPONENTS_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <opencv2/opencv.hpp>

// Define an EntityID type
using EntityID = int64_t;

// Define the type of each component map
template<typename Type>
using ComponentMap = std::unordered_map<EntityID, Type>;

using Position = glm::vec3;
using Positions = ComponentMap<Position>;

using Orientation = glm::vec3;
using Orientations = ComponentMap<Orientation>;

using Mesh = std::vector<glm::mat3x4>;
using Meshes = ComponentMap<Mesh>;

using Scale = glm::vec3;
using Scales = ComponentMap<Scale>;

using Color = cv::Scalar_<double>;
using Colors = ComponentMap<Color>;

#endif //PSR_3D_RENDERER_ENTITYCOMPONENTS_H
