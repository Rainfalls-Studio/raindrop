#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_TRANSFORM_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_TRANSFORM_HPP__

#include <Raindrop/Core/Scene/Component.hpp>

namespace Raindrop::Core::Scene::Components{
	class Transform : public Component{
		public:
			Transform() = default;
			virtual ~Transform() override = default;
			
			glm::vec3 translation;
			glm::vec3 scale;
			glm::vec3 rotation;

			virtual void UI(EngineContext& context) override;

	};
}

#endif