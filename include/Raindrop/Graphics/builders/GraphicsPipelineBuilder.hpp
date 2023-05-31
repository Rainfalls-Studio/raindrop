#ifndef __RAINDROP_GRAPHICS_BUILDERS_GRAPHICS_PIPELINE_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_GRAPHICS_PIPELINE_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Builders{
	class PipelineBuilder{
		public:
			PipelineBuilder();
			~PipelineBuilder();

			std::shared_ptr<GraphicsPipeline> build(VkAllocationCallbacks* callbacks = nullptr);

			void setDevice(const std::shared_ptr<Device>& device);
			void addShader(const std::shared_ptr<Shader>& shader);
			void setRenderPass(VkRenderPass renderPass);

		private:
			std::shared_ptr<Device> _device;
			VkRenderPass _renderPass = VK_NULL_HANDLE;
			uint32_t _subpass = 0;

			std::vector<std::shared_ptr<Shader>> _shaders;

			VkPipelineLayout createPipelineLayout(VkAllocationCallbacks* callbacks);
			void destroyPipelineLayout(VkPipelineLayout layout, VkAllocationCallbacks* callbacks);
	};
}

#endif