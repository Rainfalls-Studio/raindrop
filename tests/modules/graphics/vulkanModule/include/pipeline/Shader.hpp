#ifndef __VULKANMODULE_SHADER_HPP__
#define __VULKANMODULE_SHADER_HPP__

#include "core.hpp"
#include <shaderc/shaderc.h>
#include "ShaderCompiler.hpp"

class Shader{
	public:
		Shader(GfxAllocator& allocator, ShaderCompiler& compiler);
		~Shader();

		void setKind(shaderc_shader_kind kind);
		void setStage(VkShaderStageFlagBits stage);
		bool load(const Core::String& code);

		VkShaderStageFlagBits stage() const;
		VkPipelineShaderStageCreateInfo stageCreateInfo() const;

	private:
		GfxAllocator& _allocator;
		ShaderCompiler& _compiler;
		Device& _device;
		VkShaderStageFlagBits _stage;

		VkShaderModule _module;
};

#endif