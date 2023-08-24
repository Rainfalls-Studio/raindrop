#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SUN_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SUN_HPP__

#include <Raindrop/Graphics/ForwardShader/PushContants/common.hpp>

namespace Raindrop::Graphics::ForwardShader::PushConstants{
	struct Sun{
		alignas(16) glm::vec3 cameraPosition;
		alignas(16) glm::vec3 cameraDirection;
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 direction;
		float intensity;
	};
}

#endif