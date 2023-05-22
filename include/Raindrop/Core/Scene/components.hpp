#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene::Components{
	using Tag = std::string;

	struct Transform{
		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 rotation;
	};
}

#endif