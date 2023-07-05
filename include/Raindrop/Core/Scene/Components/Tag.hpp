#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_TAG_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_TAG_HPP__

#include <Raindrop/Core/Scene/Component.hpp>

namespace Raindrop::Core::Scene::Components{
	class Tag : public Component{
		public:
			Tag() = default;
			virtual ~Tag() override = default;

			std::string name = "Entity";
	};
}

#endif