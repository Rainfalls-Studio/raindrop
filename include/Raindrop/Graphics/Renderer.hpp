#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/InstanceBuilder.hpp>
#include <Raindrop/Graphics/PhysicalDeviceManager.hpp>

namespace Raindrop::Graphics{
	class Renderer{
		public:
			Renderer(Core::Event::EventManager& eventManager);
			~Renderer();

			std::shared_ptr<Instance> instance() const;
			std::shared_ptr<Device> device() const;

			void update();
		
		private:
			Core::Event::EventManager& _eventManager;
			std::shared_ptr<Instance> _instance;
			std::shared_ptr<PhysicalDeviceManager> _physicalDeviceManager;
			std::shared_ptr<Device> _device;
			std::shared_ptr<Window> _window;

			std::unique_ptr<Swapchain> _swapchain;
			std::vector<VkCommandBuffer> _graphicsCommandBuffers;
			VkSurfaceKHR _surface = VK_NULL_HANDLE;
			VkCommandPool _graphicsCommandPool = VK_NULL_HANDLE;

			VkQueue _graphicsQueue;
			VkQueue _presentQueue;

			void createInstance();
			void createPhysicalDeviceManager();
			void createDevice();
			void createWindow();
			void createSurface();
			void createSwapchain();
			void createGraphicsCommandPool();
			void createGraphicsCommandBuffers();
			
			void destroyGraphicsCommandBuffers();
			void destroyGraphicsCommandPool();
			
			std::shared_ptr<PhysicalDevice> findSuitablePhysicalDevice();
			VkCommandBuffer getCurrentGraphicsCommandBuffer();


			VkCommandBuffer beginFrame();
			void endFrame();
			

	};
}

#endif