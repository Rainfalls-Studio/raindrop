#ifndef __RAINDROP_GRAPHICS_BUILDERS_DESCRIPTOR_POOL_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_DESCRIPTOR_POOL_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

//TODO: add build from xml  - https://trello.com/c/1ppfxZk8/1-add-build-from-xml
namespace Raindrop::Graphics::Builders{
	class DescriptorPoolBuilder{
		public:
			DescriptorPoolBuilder();
			~DescriptorPoolBuilder();

			std::shared_ptr<DescriptorPool> build(GraphicsContext& context);

			void setFlags(VkDescriptorPoolCreateFlags flags);
			void setMaxSets(uint32_t max);
			void pushPoolSize(VkDescriptorPoolSize size);

		private:
			VkDescriptorPoolCreateFlags _flags = 0;
			uint32_t _maxSets = 0;
			std::vector<VkDescriptorPoolSize> _poolSizes;
	};
}

#endif