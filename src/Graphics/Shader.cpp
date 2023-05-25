#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/Device.hpp>

namespace Raindrop::Graphics{
	Shader::Shader(std::shared_ptr<Device> device, const std::vector<char>& code, VkAllocationCallbacks* callbacks) : _allocationCallbacks{callbacks}, _device{device}{
		createShaderModule(code);
	}

	Shader::~Shader(){
		if (_shader) vkDestroyShaderModule(_device->get(), _shader, _allocationCallbacks);
	}

	void Shader::createShaderModule(const std::vector<char>& code){
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		createInfo.codeSize = static_cast<uint32_t>(code.size());

		if (vkCreateShaderModule(_device->get(), &createInfo, _allocationCallbacks, &_shader) != VK_SUCCESS){
			throw "Failed to create shader module";
		}
	}
}