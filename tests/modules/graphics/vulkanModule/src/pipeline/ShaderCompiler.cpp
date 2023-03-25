#include "pipeline/ShaderCompiler.hpp"
#include "pipeline/Shader.hpp"

ShaderCompiler::ShaderCompiler(GfxAllocator& allocator, Device& device) : _allocator{allocator}, _device{device}{
	RAINDROP_profile_function();

	_compiler = shaderc_compiler_initialize();
	_options = shaderc_compile_options_initialize();

	shaderc_compile_options_set_target_env(_options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
}

ShaderCompiler::~ShaderCompiler(){
	RAINDROP_profile_function();
	shaderc_compiler_release(_compiler);
	shaderc_compile_options_release(_options);
}

void ShaderCompiler::addDefine(const Core::String& name, const Core::String& value){
	RAINDROP_profile_function();
	shaderc_compile_options_add_macro_definition(_options, name.str(), name.size(), value.str(), value.size());
}

void ShaderCompiler::optimization(bool optimization){
	RAINDROP_profile_function();
	shaderc_compile_options_set_optimization_level(_options, optimization ? shaderc_optimization_level_performance : shaderc_optimization_level_zero);
}

const char* langToFilename(shaderc_source_language lang){
	switch (lang){
		case shaderc_source_language_glsl: return "shader.glsl";
		case shaderc_source_language_hlsl: return "shader.hlsl";
	}
	RAINDROP_log(WARNING, GRAPHICS, "unknown shaderc source language : %d", (int)lang);
	return "shader.glsl";
}

ShaderCompiler::CompileResult ShaderCompiler::compileToSpv(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang){
	RAINDROP_profile_function();
	auto result = shaderc_compile_into_spv(_compiler, code.str(), code.size(), kind, langToFilename(lang), "main", _options);

	CompileResult out(allocator);

	if (result == nullptr){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader");
		return out;
	}

	out.errorCount = shaderc_result_get_num_errors(result);
	out.warningCount = shaderc_result_get_num_errors(result);

	if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader : %s", shaderc_result_get_error_message(result));
		return out;

	}

	out.success = true;
	out.code.resize(shaderc_result_get_length(result));
	memcpy(out.code.data(), shaderc_result_get_bytes(result), out.code.size());

	shaderc_result_release(result);
	return out;
}

ShaderCompiler::CompileResult ShaderCompiler::compileToSpvAssembly(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang){
	RAINDROP_profile_function();
	auto result = shaderc_compile_into_spv_assembly(_compiler, code.str(), code.size(), kind, langToFilename(lang), "main", _options);

	CompileResult out(allocator);

	if (result == nullptr){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader");
		return out;
	}

	out.errorCount = shaderc_result_get_num_errors(result);
	out.warningCount = shaderc_result_get_num_errors(result);

	if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader : %s", shaderc_result_get_error_message(result));
		return out;
	}

	out.success = true;
	out.code.resize(shaderc_result_get_length(result));
	memcpy(out.code.data(), shaderc_result_get_bytes(result), out.code.size());

	shaderc_result_release(result);
	return out;
}

ShaderCompiler::CompileResult ShaderCompiler::compileToPreprocessedSpv(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang){
	RAINDROP_profile_function();
	auto result = shaderc_compile_into_preprocessed_text(_compiler, code.str(), code.size(), kind, langToFilename(lang), "main", _options);

	CompileResult out(allocator);

	if (result == nullptr){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader");
		return out;
	}

	out.errorCount = shaderc_result_get_num_errors(result);
	out.warningCount = shaderc_result_get_num_errors(result);

	if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success){
		RAINDROP_log(ERROR, GRAPHICS, "failed to compile shader : %s", shaderc_result_get_error_message(result));
		return out;
	}

	out.success = true;
	out.code.resize(shaderc_result_get_length(result));
	memcpy(out.code.data(), shaderc_result_get_bytes(result), out.code.size());

	shaderc_result_release(result);

	return out;
}

shaderc_compiler_t ShaderCompiler::get() const{
	RAINDROP_profile_function();
	return _compiler;
}

shaderc_compile_options_t ShaderCompiler::options() const{
	RAINDROP_profile_function();
	return _options;
}

Device& ShaderCompiler::getDevice() const{
	RAINDROP_profile_function();
	return _device;
}

Shader* ShaderCompiler::createShader(Core::Memory::Allocator& allocator, const Core::String& filepath, shaderc_shader_kind kind, shaderc_source_language lang){
	RAINDROP_profile_function();
	Shader* shader = allocateNew<Shader>(allocator, _allocator, *this);
	if (!shader) return nullptr;

	shader->setKind(kind);
	
	Core::Filesystem::File file(filepath);
	Core::Filesystem::Filebuffer shaderFile(allocator, file);
	auto result = compileToSpv(allocator, shaderFile.content(), kind, lang);

	if (!result.success){
		deallocateDelete(allocator, *shader);
		return nullptr;
	}

	if (!shader->load(result.code)){
		deallocateDelete(allocator, *shader);
		return nullptr;
	}

	return shader;
}
