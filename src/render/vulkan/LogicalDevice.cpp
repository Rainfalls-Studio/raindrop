#include "render/vulkan/LogicalDevice.hpp"
#include "render/vulkan/CommandPool.hpp"
#include "render/vulkan/utils.hpp"
#include "core.hpp"

#include <stdexcept>
#include <cassert>

namespace rnd::render::vulkan{
	struct UniqueFamily : public PhysicalDevice::FamilyDetails{
		UniqueFamily(const PhysicalDevice::FamilyDetails &other) : PhysicalDevice::FamilyDetails(other){};
		UniqueFamily() = default;

		std::vector<float> priorities{};
	};

	void LogicalDevice::createCommandPools(LogicalDeviceBuilder &builder){
		PROFILE_FUNCTION();
		auto families = physicalDevice->getEnabledFamilies();

		for (int i=0; i<FAMILY_COUNT; i++){
			if (families[i]){
				CommandPool* pool = new CommandPool();
				CommandPoolBuilder b;
				b.setFamily(static_cast<QueueFamily>(i));
				b.setLogicalDevice(this);
				b.setFlag(builder.commandPoolFlags[i]);
				pool->init(b);
				commandPools[i] = pool;
			} else {
				commandPools[i] = nullptr;
			}
		}
	}

	void LogicalDevice::init(LogicalDeviceBuilder &builder){
		PROFILE_FUNCTION();
		assert(builder.physicalDevice != nullptr && "cannot create a logical device without a valid physical device");
		physicalDevice = builder.physicalDevice;
		Instance* instance = physicalDevice->getInstance();

		std::vector<PhysicalDevice::FamilyDetails> families = physicalDevice->getFamilies();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::vector<UniqueFamily> uniqueFamilies;

		for (UniqueFamily family : families){
			bool isUnique = true;
			queues[family.type].queueCount = builder.requiredQueuesCount[family.type];
			family.priorities = builder.queuePriorities[family.type];

			for (auto &uniqueFamilie : uniqueFamilies){
				if (uniqueFamilie.family == family.family){

					uniqueFamilie.priorities.insert(uniqueFamilie.priorities.end(), family.priorities.begin(), family.priorities.end());

					isUnique = false;
					break;
				}
			}

			if (isUnique){
				uniqueFamilies.push_back(family);
			}
		}

		for (int i=0; i<FAMILY_COUNT; i++){
			if (queues[i].queueCount == 0) continue;
			queues[i].queues = new VkQueue[queues[i].queueCount];
		}

		std::vector<float> priorities(uniqueFamilies.size());

		for (int i=0; i<uniqueFamilies.size(); i++){
			auto& queueFamily = uniqueFamilies[i];

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily.family;

			queueCreateInfo.queueCount = static_cast<uint32_t>(queueFamily.priorities.size());

			if (queueCreateInfo.queueCount > queueFamily.queueCount){
				queueCreateInfo.queueCount = queueFamily.queueCount;
			}

			queueCreateInfo.queueFamilyIndex = queueFamily.family;
			queueCreateInfo.pQueuePriorities = queueFamily.priorities.data();
			
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures features = physicalDevice->getEnabledFeatures();

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		
		createInfo.pEnabledFeatures = &features;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(builder.requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = builder.requiredExtensions.data();

		if (instance->isValidationLayersEnabled()){
			createInfo.enabledLayerCount = static_cast<uint32_t>(instance->getValidationLayers().size());
			createInfo.ppEnabledLayerNames = instance->getValidationLayers().data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateDevice(physicalDevice->getDevice(), &createInfo, nullptr, &device);

		if (result != VK_SUCCESS){
			throw (std::string("failed to create device | ") + resultToStr(result)).c_str();
		}

		uint32_t queueIndices[FAMILY_COUNT];
		for (int i=0; i<FAMILY_COUNT; i++){
			queueIndices[i] = 0;
		}

		for (auto &f : families){
			uint32_t *queueIndex = nullptr;

			for (auto &uf : uniqueFamilies){
				if (f.family == uf.family){
					queueIndex = &queueIndices[static_cast<int>(uf.type)];
					break;
				}
			}

			for (int i=0; i<builder.requiredQueuesCount[static_cast<int>(f.type)]; i++){
				uint32_t id = (*queueIndex) % physicalDevice->getFamily(f.type).queueCount;
				vkGetDeviceQueue(device, f.family, id, &queues[static_cast<size_t>(f.type)].queues[i]);
				(*queueIndex)++;
			}
		}

		createCommandPools(builder);
	}

	void LogicalDevice::shutdown(){
		PROFILE_FUNCTION();
		for (int i=0; i<FAMILY_COUNT; i++){
			if (queues[i].queueCount > 0){
				delete[] queues[i].queues;
			}
		}

		for (int i=0; i<FAMILY_COUNT; i++){
			if (commandPools[i] != nullptr) delete commandPools[i];
		}
		
		vkDestroyDevice(device, nullptr);
	}

	VkDevice LogicalDevice::getDevice() const{
		PROFILE_FUNCTION();
		return device;
	}

	void LogicalDevice::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory){
		PROFILE_FUNCTION();
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw "failed to create image !";
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physicalDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

		{
			VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
			if (result != VK_SUCCESS) {
				throw (std::string("failed to allocate image memory | ") + resultToStr(result)).c_str();
			}
		}

		{
			VkResult result = vkBindImageMemory(device, image, imageMemory, 0);
			if (result != VK_SUCCESS){
				throw (std::string("failed to bind image memory | ") + resultToStr(result)).c_str();
			}
		}
	}

	void LogicalDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory){
		PROFILE_FUNCTION();
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		{
			VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
			if (result != VK_SUCCESS){
				throw (std::string("failed to create buffer | ") + resultToStr(result)).c_str();
			}
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = physicalDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

		{
			VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
			if (result != VK_SUCCESS){
				throw (std::string("failed to allocate buffer memory | ") + resultToStr(result)).c_str();
			}
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	VkQueue LogicalDevice::getQueue(QueueFamily family, uint32_t index){
		PROFILE_FUNCTION();
		return queues[family].queues[index];
	}

	Instance* LogicalDevice::getInstance(){
		PROFILE_FUNCTION();
		return physicalDevice->getInstance();
	}

	CommandPool* LogicalDevice::getCommandPool(QueueFamily family){
		PROFILE_FUNCTION();
		return commandPools[family];
	}

	PhysicalDevice* LogicalDevice::getPhysicalDevice(){
		PROFILE_FUNCTION();
		return physicalDevice;
	}
}