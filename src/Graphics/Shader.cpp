#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	Shader::Shader(GraphicsContext& context, const std::vector<char>& code, VkShaderStageFlagBits stage) : _context{context}, _stage{stage}{
		createShaderModule(code);
	}

	Shader::~Shader(){
		if (_shader) vkDestroyShaderModule(_context.device.get(), _shader, _context.allocationCallbacks);
	}

	void Shader::createShaderModule(const std::vector<char>& code){
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		createInfo.codeSize = static_cast<uint32_t>(code.size());

		if (vkCreateShaderModule(_context.device.get(), &createInfo, _context.allocationCallbacks, &_shader) != VK_SUCCESS){
			throw "Failed to create shader module";
		}
	}

	VkShaderModule Shader::get() const{
		return _shader;
	}

	VkShaderStageFlagBits Shader::stage() const{
		return _stage;
	}

}