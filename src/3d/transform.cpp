#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "transform.h"

namespace Impacto {

void Transform::SetRotationFromEuler(glm::vec3 eulerZYX) {
  Rotation =
      glm::quat_cast(glm::eulerAngleZYX(eulerZYX.z, eulerZYX.y, eulerZYX.x));
}

glm::mat4 Transform::Matrix() {
  glm::mat4 result = glm::translate(glm::mat4(1.0), Position);
  result = result * glm::mat4_cast(Rotation);
  result = glm::scale(result, Scale);
  return result;
}

}  // namespace Impacto