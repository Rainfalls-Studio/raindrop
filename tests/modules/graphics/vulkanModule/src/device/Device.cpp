#include "device/Device.hpp"

Device::Device(GfxAllocator& allocator) : _allocator{allocator}, _requiredExtensions(_allocator.get()), _requiredLayers(_allocator.get()), _queues(_allocator.get()), _commandPools(_allocator.get()){
	RAINDROP_profile_function();
	_loaded = false;
	_device = VK_NULL_HANDLE;
}

Device::~Device(){

	{
		auto node = _commandPools.front();
		while (node){
			auto& pool = node->_pool;
			if (pool != VK_NULL_HANDLE) vkDestroyCommandPool(_device, pool, _allocator.callback());
			pool = VK_NULL_HANDLE;
			node++;
		}
	}

	_commandPools.clear();

	if (_device != VK_NULL_HANDLE) vkDestroyDevice(_device, _allocator.callback());
	_device = VK_NULL_HANDLE;

	_loaded = false;
}

void Device::requiredExtensions(const char* extension){
	RAINDROP_profile_function();
	_requiredExtensions.push(extension);
}

void Device::requireLayer(const char* layer){
	RAINDROP_profile_function();
	_requiredLayers.push(layer);
}

void Device::init(PhysicalDevice& physicalDevice){
	RAINDROP_profile_function();
	RAINDROP_log(INFO, GRAPHICS, "creating a new vulkan device");

	_physicalDevice = &physicalDevice;
	if (_physicalDevice->instance().validation()){
		_requiredLayers.push(_physicalDevice->instance().validationLayers());
	}

	VkDeviceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	info.enabledExtensionCount = _requiredExtensions.size();
	info.ppEnabledExtensionNames = _requiredExtensions.data();

	info.enabledLayerCount = _requiredLayers.size();
	info.ppEnabledLayerNames = _requiredLayers.data();

	VkPhysicalDeviceFeatures features = _physicalDevice->getFeatures();
	info.pEnabledFeatures = &features;

	Array<VkDeviceQueueCreateInfo> queueCreateInfos(_allocator.stack(), _physicalDevice->getFamiliesProperties().size());
	pouplateQueueCreateInfo(queueCreateInfos);

	float priority = 0.f;
	for (uint32 i=0; i<queueCreateInfos.size(); i++){
		queueCreateInfos[i].pQueuePriorities = &priority;
	}

	info.queueCreateInfoCount = queueCreateInfos.size();
	info.pQueueCreateInfos = queueCreateInfos.data();

	VkResult result = vkCreateDevice(_physicalDevice->get(), &info, _allocator.callback(), &_device);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan logical device : %s", string_VkResult(result));
		return;
	}

	_queues.resize(queueCreateInfos.size());
	RAINDROP_log(TRACE, GRAPHICS, "getting vulkan device queues");
	for (uint32 i=0; i<queueCreateInfos.size(); i++){
		vkGetDeviceQueue(_device, i, 0, &_queues[i]);
	}

	RAINDROP_log(INFO, GRAPHICS, "vulkan device intilized with success");
	_loaded = true;
}

void Device::pouplateQueueCreateInfo(Array<VkDeviceQueueCreateInfo>& queueCreateInfo){
	RAINDROP_profile_function();

	const auto& properties = _physicalDevice->getFamiliesProperties();
	queueCreateInfo.resize(properties.size());

	for (uint32 i=0; i<properties.size(); i++){
		auto& info = queueCreateInfo[i];
		const auto& prop = properties[i];

		info = {};

		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.queueCount = 1;
		info.queueFamilyIndex = i;
	}
}

PhysicalDevice& Device::physicalDevice(){
	RAINDROP_profile_function();
	return *_physicalDevice;
}

Instance& Device::instance(){
	RAINDROP_profile_function();
	return _physicalDevice->instance();
}

const Array<VkQueue>& Device::queues() const{
	RAINDROP_profile_function();
	return _queues;
}

VkQueue Device::getQueue(uint32 index) const{
	RAINDROP_profile_function();
	return _queues[index];
}

bool Device::loaded() const{
	RAINDROP_profile_function();
	RAINDROP_profile_function();
	return _loaded;
}

VkDevice Device::get() const{
	RAINDROP_profile_function();
	return _device;
}

const Array<const char*> Device::requiredExtensions() const{
	RAINDROP_profile_function();
	return _requiredExtensions;
}

const Array<const char*> Device::requiredLayers() const{
	RAINDROP_profile_function();
	return _requiredLayers;
}

bool Device::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image,VkDeviceMemory &imageMemory){
	RAINDROP_profile_function();
	if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan image");
		return false;
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _physicalDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		RAINDROP_log(ERROR, GRAPHICS, "failed to allocate image memory!");
		return false;
	}

	if (vkBindImageMemory(_device, image, imageMemory, 0) != VK_SUCCESS) {
		RAINDROP_log(ERROR, GRAPHICS, "failed to bind image memory");
		return false;
	}
	return true;
}

VkCommandPool Device::getCommandPool(VkCommandPoolCreateFlags flags, uint32 familyIndex){
	RAINDROP_profile_function();
	auto pool = _commandPools.front();

	while (pool){
		if (pool->_flags & flags && pool->_familyIndex == familyIndex) return pool->_pool;
		pool++;
	}

	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.flags = flags;
	info.queueFamilyIndex = familyIndex;
	
	CommandPool commandPool;
	VkResult result = vkCreateCommandPool(_device, &info, _allocator.callback(), &commandPool._pool);
	
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan command pool");
		return VK_NULL_HANDLE;
	}

	commandPool._familyIndex = familyIndex;
	commandPool._flags = flags;

	_commandPools.push(commandPool);
	return commandPool._pool;
}
