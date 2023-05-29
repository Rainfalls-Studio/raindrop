#ifndef __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_PIPELINE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsPipeline{
		public:
			GraphicsPipeline(const VkGraphicsPipelineCreateInfo& info, std::vector<std::shared_ptr<Shader>> shaders, VkAllocationCallbacks* allocationCallbacks = nullptr);
			~GraphicsPipeline();

		private:
			VkAllocationCallbacks* _allocationCallbacks;
			std::vector<std::shared_ptr<Shader>> _shaders;
	};
}

#endif