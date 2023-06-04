#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/InstanceBuilder.hpp>
#include <Raindrop/Graphics/PhysicalDeviceManager.hpp>
#include <Raindrop/Graphics/factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/factory/GraphicsPipelineFactory.hpp>

namespace Raindrop::Graphics{
	class Renderer{
		friend class GUI;
		public:
			Renderer(Core::Event::EventManager& eventManager, Core::Asset::AssetManager& assetManager, Core::Registry::Registry& registry, Core::Scene::Scene& scene);
			~Renderer();

			std::shared_ptr<Instance> instance() const;
			std::shared_ptr<Device> device() const;

			void update();

			VkCommandBuffer beginSingleUseTransfertCommandBuffer();
			void endSingleUseTransfertCommandBuffer(VkCommandBuffer commandBuffer);

		
		private:
			Core::Event::EventManager& _eventManager;
			Core::Asset::AssetManager& _assetManager;
			Core::Scene::Scene& _scene;
			Core::Registry::Registry& _registry;

			std::unique_ptr<GUI> _gui;

			std::shared_ptr<Instance> _instance;
			std::shared_ptr<PhysicalDeviceManager> _physicalDeviceManager;
			std::shared_ptr<Device> _device;
			std::shared_ptr<Window> _window;

			std::unique_ptr<Swapchain> _swapchain;
			std::vector<VkCommandBuffer> _graphicsCommandBuffers;
			VkSurfaceKHR _surface = VK_NULL_HANDLE;
			VkCommandPool _graphicsCommandPool = VK_NULL_HANDLE;
			VkCommandPool _transfertCommandPool = VK_NULL_HANDLE;

			std::shared_ptr<Factory::GraphicsPipelineFactory> _graphicsPipelineFactory;
			std::shared_ptr<Factory::ShaderFactory> _shaderFactory; 

			VkQueue _graphicsQueue;
			VkQueue _transfertQueue;
			VkQueue _presentQueue;

			void createInstance();
			void createPhysicalDeviceManager();
			void createDevice();
			void createWindow();
			void createSurface();
			void createSwapchain();
			void createGraphicsCommandPool();
			void createGraphicsCommandBuffers();
			void createTransfertCommandPool();
			
			void destroyGraphicsCommandBuffers();
			void destroyGraphicsCommandPool();
			void destroyTransfertCommandPool();

			void registerFactories();
			void registerShaderFactory();
			void registerGraphicsPipelineFactory();
			void eraseFactories();

			void drawEntityAndChilds(Core::Scene::Entity& entity, VkCommandBuffer commandBuffer, VkPipelineLayout layout);
			
			std::shared_ptr<PhysicalDevice> findSuitablePhysicalDevice();
			VkCommandBuffer getCurrentGraphicsCommandBuffer();

			VkCommandBuffer beginFrame();
			void endFrame();
	};

	struct PushConstant{
		glm::mat4 viewTransform;
		glm::mat4 localTransform;
	};
}

#endif