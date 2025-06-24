#pragma once

#include <Raindrop/pch.pch>
#include <Raindrop/Core/Internal/Asset/Asset.hpp>
#include <vulkan/vulkan_core.h>

namespace Raindrop::Graphics{
	class Context;
	namespace Backend::Vulkan{
		class ShaderModule{
			public:
				ShaderModule(Context& context, VkShaderModule module) noexcept;
				~ShaderModule();

				ShaderModule(const ShaderModule&) = delete;
				ShaderModule& operator=(const ShaderModule&) = delete;

				ShaderModule(ShaderModule&& other);
				ShaderModule& operator=(ShaderModule&& other);

				friend void swap(ShaderModule& A, ShaderModule& B);

				const VkShaderModule& get() const noexcept;
		
			private:
				Context* _context;
				VkShaderModule _module;
		};
	}
}