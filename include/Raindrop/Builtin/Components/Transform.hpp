#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL // quaternions
#include <glm/gtx/quaternion.hpp>

namespace Raindrop::Builtin::Components{
    class Transform{
        public:
            glm::vec3 translation = {0.f, 0.f, 0.f};
            glm::vec3 scale = {1.f, 1.f, 1.f};
            glm::quat rotation = glm::identity<glm::quat>();

            glm::mat4 localTransform = glm::mat4(1.f);
            glm::mat4 worldTransform = glm::mat4(1.f);

            // When marked dirty, the transform will be recalculated
            bool dirty = false;
    };
}