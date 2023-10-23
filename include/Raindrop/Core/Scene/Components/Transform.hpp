#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_TRANSFORM_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_TRANSFORM_HPP__

#include <Raindrop/Core/Scene/Component.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Raindrop::Core::Scene::Components{
	class Transform : public Component{
		public:
			Transform() = default;
			virtual ~Transform() override = default;
			
			glm::vec3 translation = glm::vec3(0, 0, 0);
			glm::vec3 scale = glm::vec3(1, 1, 1);
			glm::quat rotation = glm::quat(1.f, 1.f, 0.f, 0.f);

	};
}

#endif