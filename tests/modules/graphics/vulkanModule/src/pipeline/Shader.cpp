#include "pipeline/Shader.hpp"

VkShaderStageFlagBits kindToStage(shaderc_shader_kind kind){
	switch (kind){
		case shaderc_vertex_shader: return VK_SHADER_STAGE_VERTEX_BIT;
		case shaderc_fragment_shader: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case shaderc_compute_shader: return VK_SHADER_STAGE_COMPUTE_BIT;
		case shaderc_geometry_shader: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case shaderc_tess_control_shader: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case shaderc_tess_evaluation_shader: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case shaderc_raygen_shader: return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		case shaderc_anyhit_shader: return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		case shaderc_closesthit_shader: return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		case shaderc_intersection_shader: return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		case shaderc_callable_shader: return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
		case shaderc_task_shader: return VK_SHADER_STAGE_TASK_BIT_NV;
		case shaderc_mesh_shader: return VK_SHADER_STAGE_MESH_BIT_NV;
	}
	RAINDROP_log(WARNING, GRAPHICS, "unknown shaderc shader kind : %d", static_cast<int>(kind));
	return VK_SHADER_STAGE_VERTEX_BIT;
}

Shader::Shader(GfxAllocator& allocator, ShaderCompiler& compiler) : _allocator{allocator}, _compiler{compiler}, _device{compiler.getDevice()}{
	_module = VK_NULL_HANDLE;
}

Shader::~Shader(){
	if (_module) vkDestroyShaderModule(_device.get(), _module, _allocator.callback());
	_module = VK_NULL_HANDLE;
}

void Shader::setKind(shaderc_shader_kind kind){
	_stage = kindToStage(kind);
}

void Shader::setStage(VkShaderStageFlagBits stage){
	_stage = stage;
}

bool Shader::load(const Core::String& code){
	RAINDROP_profile_function();

	VkShaderModuleCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize = code.size();
	info.pCode = reinterpret_cast<const uint32_t*>(code.str());

	VkResult result = vkCreateShaderModule(_device.get(), &info, _allocator.callback(), &_module);

	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan shader module %s", string_VkResult(result));
		return false;
	}
}

VkShaderStageFlagBits Shader::stage() const{
	return _stage;
}

VkPipelineShaderStageCreateInfo Shader::stageCreateInfo() const{
	RAINDROP_profile_function();

	VkPipelineShaderStageCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.module = _module;
	info.pName = "main";
	info.stage = _stage;

	return info;
}