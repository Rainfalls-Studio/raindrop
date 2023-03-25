#ifndef __VULKANMODULE_SHADER_COMPILER_HPP__
#define __VULKANMODULE_SHADER_COMPILER_HPP__

#include "core.hpp"
#include "device/Device.hpp"
#include <shaderc/shaderc.h>

class Shader;
class ShaderCompiler{
	public:
		ShaderCompiler(GfxAllocator& allocator, Device& device);
		~ShaderCompiler();

		void addDefine(const Core::String& name, const Core::String& value);
		void optimization(bool optimization);

		struct CompileResult{
			Core::String code;
			uint32 warningCount;
			uint32 errorCount;
			bool success;

			CompileResult(Allocator& allocator) : code{allocator}{success = false;}
		};
		
		CompileResult compileToSpv(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang);
		CompileResult compileToSpvAssembly(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang);
		CompileResult compileToPreprocessedSpv(Core::Memory::Allocator& allocator, const Core::String& code, shaderc_shader_kind kind, shaderc_source_language lang);
		Shader* createShader(Core::Memory::Allocator& allocator, const Core::String& filepath, shaderc_shader_kind kind, shaderc_source_language lang);

		shaderc_compiler_t get() const;
		shaderc_compile_options_t options() const;
		Device& getDevice() const;

	private:
		GfxAllocator& _allocator;
		Device& _device;
		shaderc_compiler_t _compiler;
		shaderc_compile_options_t _options;
};

#endif