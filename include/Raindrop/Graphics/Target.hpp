#ifndef __RAINDROP_GRAPHICS_TARGET_HPP__
#define __RAINDROP_GRAPHICS_TARGET_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Target{
		public:
			Target() = default;
			virtual ~Target() = default;

			virtual VkRenderPass renderPass() const = 0;
			virtual uint32_t attachmentCount() const = 0;
	};
}

#endif