#pragma once

#include <VkBootstrap.h>
#include "../Device.hpp"
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Buffer.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "Raindrop/Window/Window.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
	class Device : public Backend::Device{
		public:
			Device(const DeviceConfig& config);
			virtual ~Device() override;

			virtual std::shared_ptr<Backend::Buffer> createBuffer(const Buffer::Description& description) override;
			virtual std::shared_ptr<Backend::CommandList> createCommandList() override;
			virtual std::shared_ptr<Backend::DescriptorSet> createDescriptorSet() override;
			virtual std::shared_ptr<Backend::Framebuffer> createFramebuffer() override;
			virtual std::shared_ptr<Backend::GraphicsPipeline> createGraphicsPipeline() override;
			virtual std::shared_ptr<Backend::RenderPass> createRenderPass() override;
			virtual std::shared_ptr<Backend::Sampler> createSampler() override;
			virtual std::shared_ptr<Backend::Shader> createShader() override;
			virtual std::shared_ptr<Backend::Texture> createTexture() override;

		private:
			Context _context;

			Engine& _gfxEngine;
            std::shared_ptr<Asset::Manager> _assetManager;
            std::shared_ptr<Window::Manager> _windowManager;

			void createLogger();
			void createInstance(std::shared_ptr<Window::SurfaceProviders::Vulkan> surfaceProvider);
			void findPhysicalDevice(std::shared_ptr<Window::SurfaceProviders::Vulkan> surfaceProvider);
			void createDevice();
			void getQueues();

			void createSwapchain(std::shared_ptr<Window::Window> window);
	};
}