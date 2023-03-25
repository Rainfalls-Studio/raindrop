#ifndef __VULKANMODULE_PIPELINE_HPP__
#define __VULKANMODULE_PIPELINE_HPP__

#include "core.hpp"
#include "Allocator.hpp"
#include "device/Device.hpp"

class Shader;
class Pipeline{
	static constexpr uint32 STAGE_BIT_COUNT = sizeof(VkShaderStageFlags) * 8;

	public:	
		Pipeline(GfxAllocator& allocator, Device& device);
		~Pipeline();

		void pushShader(Shader* shader);
		bool build();
		void setRenderPass(VkRenderPass renderPass);
	
	private:
		GfxAllocator& _allocator;
		Device& _device;
		Shader* _shaders[STAGE_BIT_COUNT];
		VkPipeline _pipeline;
		VkPipelineLayout _layout;
		VkRenderPass _renderPass;

		bool createPipeline();
		bool createLayout();

		uint32 shaderCount();
		void populateStagesInfo(Array<VkPipelineShaderStageCreateInfo>& infos);
};

#endif