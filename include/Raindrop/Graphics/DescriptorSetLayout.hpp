#ifndef __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUT_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class DescriptorSetLayout{
		public:
			DescriptorSetLayout(GraphicsContext& context, VkDescriptorSetLayoutCreateInfo& info);
			~DescriptorSetLayout();

			VkDescriptorSetLayout get() const;
			
		private:
			GraphicsContext& _context;
			VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
	};
}

#endif