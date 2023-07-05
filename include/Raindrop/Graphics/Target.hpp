#ifndef __RAINDROP_GRAPHICS_TARGET_HPP__
#define __RAINDROP_GRAPHICS_TARGET_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Target{
		public:
			Target() = default;
			virtual ~Target() = default;

			virtual VkRenderPass renderPass() const {return VK_NULL_HANDLE;}
			virtual uint32_t attachmentCount() const {return 0;}
	};
}

#endif