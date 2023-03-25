#include "device/Instance.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

Instance::Instance(GfxAllocator& allocator, bool validation) : _validation{validation}, _requiredExtensions(allocator.get()), _validationLayers(allocator.get()), _allocator(&allocator){
	RAINDROP_profile_function();
	RAINDROP_log(INFO, GRAPHICS, "creating vulkan instance");
	_loaded = false;
	_instance = VK_NULL_HANDLE;

	VkApplicationInfo appInfo{};
	initApplicationInfo(appInfo);
	initRequiredExtentions();
	
	VkInstanceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appInfo;

	if (_validation){
		initValidationLayers();
		info.ppEnabledLayerNames = _validationLayers.data();
		info.enabledLayerCount = _validationLayers.size();
	} else {
		info.enabledLayerCount = 0;
	}

	info.ppEnabledExtensionNames = _requiredExtensions.data();
	info.enabledExtensionCount = _requiredExtensions.size();

	if (vkCreateInstance(&info, nullptr, &_instance) != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create a vulkan instance");
		return;
	}

	_loaded = checkRequiredExtentions();
	if (!_loaded){
		RAINDROP_log(INFO, GRAPHICS, "vulkan instance created with success");
	}
}

Instance::~Instance(){
	RAINDROP_profile_function();
	if (_instance != VK_NULL_HANDLE) vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;
}

void Instance::initApplicationInfo(VkApplicationInfo& info){
	RAINDROP_profile_function();
	info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	info.pApplicationName = "raindrop default vulkan module";
	info.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
	info.pEngineName = "Raindrop";
	info.engineVersion = VK_MAKE_API_VERSION(0, RAINDROP_VERSION_MAJOR, RAINDROP_VERSION_MINOR, RAINDROP_VERSION_PATCH);
	info.apiVersion = VK_API_VERSION_1_2;
}

void Instance::initValidationLayers(){
	RAINDROP_profile_function();
	_validationLayers.push("VK_LAYER_KHRONOS_validation");
}

void Instance::initRequiredExtentions(){
	RAINDROP_profile_function();
	uint32 count;
	SDL_Vulkan_GetInstanceExtensions(getInitWindow(), &count, nullptr);

	_requiredExtensions.resize(count);
	SDL_Vulkan_GetInstanceExtensions(getInitWindow(), &count, _requiredExtensions.data());
}

bool Instance::checkRequiredExtentions(){
	RAINDROP_profile_function();
	RAINDROP_log(TRACE, GRAPHICS, "checking instance extensions");
	uint32 count;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

	Array<VkExtensionProperties> extensions(_allocator->stack(), count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

	bool extensionsFound = true;
	for (usize i=0; i<_requiredExtensions.size(); i++){
		bool found = false;
		const auto& required = _requiredExtensions[i];

		RAINDROP_log(TRACE, GRAPHICS, "looking for : %s vulkan instance extention", required);

		for (usize j=0; j<extensions.size(); j++){
			const auto& extension = extensions[j];

			if (strcmp(extension.extensionName, required) == 0){
				found = true;
				break;
			}
		}

		if (!found){
			extensionsFound = false;
			RAINDROP_log(ERROR, GRAPHICS, "missing vulkan extension : %s", required);
		}
	}

	return extensionsFound;
}

bool Instance::loaded() const{
	RAINDROP_profile_function();
	return _loaded;
}

VkInstance Instance::get() const{
	RAINDROP_profile_function();
	return _instance;
}

bool Instance::validation() const{
	RAINDROP_profile_function();
	return _validation;
}

Array<const char*>& Instance::validationLayers(){
	RAINDROP_profile_function();
	return _validationLayers;
}
