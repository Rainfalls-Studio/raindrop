#ifndef __RAINDROP_GRAPHICS_SHADER_HPP__
#define __RAINDROP_GRAPHICS_SHADER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Shader : public Core::Asset::Asset{
		public:
			Shader(std::shared_ptr<Device> device, const std::vector<char>& code, VkShaderStageFlagBits stage, VkAllocationCallbacks* callbacks = nullptr);
			virtual ~Shader() override;

			VkShaderModule get() const;
			VkShaderStageFlagBits stage() const;

		private:
			VkAllocationCallbacks* _allocationCallbacks = nullptr;
			std::shared_ptr<Device> _device;
			VkShaderModule _shader = VK_NULL_HANDLE;
			VkShaderStageFlagBits _stage;

			void createShaderModule(const std::vector<char>& code);
	};
}

#endif