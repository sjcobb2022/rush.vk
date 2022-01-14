#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace volk {

// from: https://stackoverflow.com/a/57595105
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hashCombine(seed, rest), ...);
};

glm::quat clampQuatX(glm::quat q, float x);

}  // namespace volk