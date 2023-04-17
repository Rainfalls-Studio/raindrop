#ifndef __RAINDORP_CORE_SCENE_COMPONENTS_HPP__
#define __RAINDORP_CORE_SCENE_COMPONENTS_HPP__

#include <common.hpp>
#include <Core/Maths/Maths.hpp>

namespace Raindrop::Core::Scene::Components{
	struct Transform{
		Maths::vec3<float> translation;
		Maths::vec3<float> rotation;
		Maths::vec3<float> scale;
	};
}

#endif