#ifndef __RAINDROP_CORE_SCENE_COMPONENT_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Component{
		public:
			Component() = default;
			virtual ~Component() = default;

			virtual void UI(EngineContext& context){};
	};
}

#endif