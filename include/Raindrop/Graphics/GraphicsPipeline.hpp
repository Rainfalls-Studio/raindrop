#ifndef __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsPipeline : public Core::Asset::Asset{
		public:
			GraphicsPipeline(GraphicsContext& context, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders);
			virtual ~GraphicsPipeline() override;

			void bind(VkCommandBuffer commandBuffer);
			
			VkPipeline pipeline() const;
			VkPipelineLayout layout() const;

		private:
			GraphicsContext& _context;
			std::vector<std::shared_ptr<Shader>> _shaders;

			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			VkPipeline _pipeline = VK_NULL_HANDLE;
	};
}

#endif