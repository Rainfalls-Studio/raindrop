#include <Raindrop/Graphics/PhysicalDeviceManager.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/builders/DeviceBuilder.hpp>

namespace Raindrop::Graphics{
	PhysicalDeviceManager::PhysicalDeviceManager(std::shared_ptr<Instance> instance, VkAllocationCallbacks* callbacks) : _instance{instance}{
		createPhysicalDevices();
	}

	PhysicalDeviceManager::~PhysicalDeviceManager(){

	}

	void PhysicalDeviceManager::createPhysicalDevices(){
		uint32_t count;
		vkEnumeratePhysicalDevices(_instance->get(), &count, nullptr);

		std::vector<VkPhysicalDevice> devices(count);
		vkEnumeratePhysicalDevices(_instance->get(), &count, devices.data());

		for (auto d : devices){
			std::shared_ptr<PhysicalDevice> physicalDevice;

			try{
				physicalDevice = std::make_shared<PhysicalDevice>(d);
			} catch (std::exception &e){
				continue;
			}

			_devices.push_back(physicalDevice);
		}

	}

	std::shared_ptr<Instance> PhysicalDeviceManager::instance() const{
		return _instance;
	}

	const std::list<std::shared_ptr<PhysicalDevice>>& PhysicalDeviceManager::devices() const{
		return _devices;
	}
}