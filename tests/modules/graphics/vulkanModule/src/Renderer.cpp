#include "core.hpp"
#include "Renderer.hpp"
#include "Context.hpp"
#include "pipeline/ShaderCompiler.hpp"
#include "pipeline/Shader.hpp"
#include "pipeline/Pipeline.hpp"

#define VALIDATION false

Renderer::Renderer(Allocator& allocator) : gfx::Renderer(), _allocator(allocator), _instance(_allocator, VALIDATION), _physicalDevices(allocator), _device(_allocator){
	RAINDROP_profile_function();
	_loaded = true;

	if (!_instance.loaded()){
		_loaded = false;
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan instance");
		return;
	}

	PhysicalDevice::getPhysicalDevices(_physicalDevices, _allocator, _instance);

	_device.requiredExtensions(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	_device.init(*(_physicalDevices[0]));
}

Renderer::~Renderer(){
	RAINDROP_profile_function();
	vkDeviceWaitIdle(_device.get());
	for (uint32 i=0; i<_physicalDevices.size(); i++){
		Core::Memory::deallocateDelete(_allocator.get(), *_physicalDevices[i]);
	}
}

gfx::Context* Renderer::createContext(Allocator& allocator, gfx::Window& window){
	RAINDROP_profile_function();
	return allocateNew<Context>(allocator, _device, _allocator, (Window&)window);
}

const char* Renderer::getAPI() const{
	RAINDROP_profile_function();
	return "Vulkan";
}

bool Renderer::loaded() const{
	RAINDROP_profile_function();
	return _loaded;
}

RAINDROP_MODULE gfx::Renderer* createRenderer(Allocator& allocator){
	RAINDROP_profile_function();
	return allocateNew<Renderer>(allocator, allocator);
}