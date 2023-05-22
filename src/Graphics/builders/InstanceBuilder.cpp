#include <Raindrop/Graphics/builders/InstanceBuilder.hpp>
#include <SDL2/SDL_vulkan.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
		fprintf(stderr, "%s\n", pCallbackData->pMessage);
		
		return VK_FALSE;
	}

	return VK_FALSE;
}

static inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator){

	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}	
}

namespace Raindrop::Graphics::Builders{
	InstanceBuilder::InstanceBuilder(){

	}

	InstanceBuilder::~InstanceBuilder(){
		
	}

	InstanceBuilder& InstanceBuilder::setApplicationName(const std::string& name){
		_applicationName = name;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::setEngineName(const std::string& name){
		_engineName = name;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::setApplicationVersion(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch){
		_applicationVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch){
		_engineVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::setVulkanAPIVersion(uint32_t version){
		_apiVersion = version;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::requireLayer(const char* layer){
		_requiredLayers.push_back(layer);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::requireExtension(const char* extension){
		_requiredExtensions.push_back(extension);
		return *this;
	}

	std::shared_ptr<Instance> InstanceBuilder::build(VkAllocationCallbacks* callbacks){

		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		std::vector<const char*> layers;
		layers.reserve(_requiredValidationLayers.size() + _requiredLayers.size());

		if (hasValidation()){
			checkValidationLayers();
			setupDebugMessenger(debugCreateInfo);
			info.pNext = &debugCreateInfo;

			layers.insert(layers.end(), _requiredValidationLayers.begin(), _requiredValidationLayers.end());
		}

		layers.insert(layers.end(), _requiredLayers.begin(), _requiredLayers.end());

		info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		info.ppEnabledLayerNames = static_cast<const char**>(layers.data());
		
		info.enabledExtensionCount = static_cast<uint32_t>(_requiredExtensions.size());
		info.ppEnabledExtensionNames = static_cast<const char**>(_requiredExtensions.data());

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.applicationVersion = _applicationVersion;
		appInfo.engineVersion = _engineVersion;
		appInfo.apiVersion = _apiVersion;
		appInfo.pApplicationName = _applicationName.c_str();
		appInfo.pEngineName = _engineName.c_str();

		info.pApplicationInfo = &appInfo;

		checkExtensions();
		checkLayers();
		
		std::shared_ptr<Instance> instance;

		try{
			instance = std::make_shared<Instance>(info, callbacks);
		} catch (const std::exception &e){
			throw e;
		}
		
		return instance;
	}

	void InstanceBuilder::checkExtensions(){
		std::set<const char*> extensions(_requiredExtensions.begin(), _requiredExtensions.end());

		uint32_t extensionsCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionsProperties(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionsProperties.data());

		for(uint32_t i=0; i<extensionsCount; i++){
			auto& properties = extensionsProperties[i];
			extensions.erase(properties.extensionName);
		}
	}

	void InstanceBuilder::checkLayers(){
		std::set<const char*> layers(_requiredLayers.begin(), _requiredLayers.end());

		uint32_t layersCount;
		vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

		std::vector<VkLayerProperties> extensionsProperties(layersCount);
		vkEnumerateInstanceLayerProperties(&layersCount, extensionsProperties.data());

		for(uint32_t i=0; i<layersCount; i++){
			auto& properties = extensionsProperties[i];
			layers.erase(properties.layerName);
		}

		if (!layers.empty()){
			throw MissingLayer(*layers.begin());
		}
	}

	InstanceBuilder& InstanceBuilder::requireGraphicsExtensions(){
		SDL_Window* window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN | SDL_WINDOW_SKIP_TASKBAR);

		if (!window){
			std::stringstream errMessage;
			errMessage << "failed to create SDL2 window (required for vulkan instance extensions) : " << SDL_GetError();
			throw std::runtime_error(errMessage.str());
		}

		uint32_t extensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr)){
			throw std::runtime_error("failed to querry SDL2 vulkan instance extensions");
		}

		std::vector<const char*> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

		_requiredExtensions.insert(_requiredExtensions.end(), extensions.begin(), extensions.end());

		SDL_DestroyWindow(window);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::requireValidationLayer(const char* layer){
		_requiredValidationLayers.push_back(layer);
		return *this;
	}

	void InstanceBuilder::checkValidationLayers(){
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> availableLayers(count);
		vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

		for (const auto &layerName : _requiredValidationLayers){
			bool layerFound = false;

			for (const auto &layerProperties : availableLayers){
				if (strcmp(layerName, layerProperties.layerName) == 0){
					layerFound = true;
					break;
				}
			}

			if (!layerFound){
				throw MissingLayer(layerName);
			}
		}
	}

	bool InstanceBuilder::hasValidation() const{
		return !_requiredValidationLayers.empty();
	}

	void InstanceBuilder::setupDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& info){
		info = {};
		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		info.pfnUserCallback = debugCallback;
		info.pUserData = nullptr;
	}
}