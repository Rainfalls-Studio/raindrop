#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_CAMERA_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_CAMERA_HPP__

#include <Raindrop/Core/Scene/Components/common.hpp>

namespace Raindrop::Core::Scene::Components{
	class Camera : public Component{
		public:
			static constexpr int ORTHOGRAPHIC_MODE = 0;
			static constexpr int PERSPECTIVE_MODE = 1;

			Camera();
			virtual ~Camera() override;

			glm::mat4 projection{1.f};
			glm::mat4 view{1.f};
			glm::mat4 viewProjection{1.f};

			virtual void UI(EngineContext& context) override;
			void update(const Transform& transform);
	};
}

#endif