#include <Raindrop/Graphics/Instance.hpp>

namespace Raindrop::Graphics{
	Instance::Instance(VkInstanceCreateInfo& info, VkAllocationCallbacks* callbacks){
		if (vkCreateInstance(&info, callbacks, &_instance) != VK_SUCCESS){
			throw std::runtime_error("failed to create vulkan instance");
		}
	}

	Instance::~Instance(){
		if (_instance){
			vkDestroyInstance(_instance, _allocatorCallbacks);
			_instance = VK_NULL_HANDLE;
		}
	}
	
	VkInstance Instance::get() const{
		return _instance;
	}
}