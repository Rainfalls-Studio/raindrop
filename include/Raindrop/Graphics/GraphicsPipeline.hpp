#ifndef __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsPipeline : public Core::Asset::Asset{
		public:
			GraphicsPipeline(const std::shared_ptr<Device>& device, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders, VkAllocationCallbacks* allocationCallbacks = nullptr);
			virtual ~GraphicsPipeline() override;

			void bind(VkCommandBuffer commandBuffer);
			

			VkPipeline pipeline() const;
			VkPipelineLayout layout() const;

		private:
			VkAllocationCallbacks* _allocationCallbacks;
			std::shared_ptr<Device> _device;
			std::vector<std::shared_ptr<Shader>> _shaders;

			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			VkPipeline _pipeline = VK_NULL_HANDLE;
	};
}

#endif