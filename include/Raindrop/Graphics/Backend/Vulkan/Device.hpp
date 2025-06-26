#pragma once

#include <VkBootstrap.h>
#include "../Device.hpp"
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "Raindrop/Window/Window.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
	class Device : public Backend::Device{
		public:
			Device(const DeviceConfig& config);
			virtual ~Device() override;

			virtual std::shared_ptr<Buffer> createBuffer() override;
			virtual std::shared_ptr<CommandList> createCommandList() override;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet() override;
			virtual std::shared_ptr<Framebuffer> createFramebuffer() override;
			virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline() override;
			virtual std::shared_ptr<RenderPass> createRenderPass() override;
			virtual std::shared_ptr<Sampler> createSampler() override;
			virtual std::shared_ptr<Shader> createShader() override;
			virtual std::shared_ptr<Texture> createTexture() override;

		private:
			Context _context;

			Engine& _gfxEngine;
            std::shared_ptr<Asset::Manager> _assetManager;
            std::shared_ptr<Window::Manager> _windowManager;

			void createLogger();
			void createInstance(std::shared_ptr<Window::SurfaceProviders::Vulkan> surfaceProvider);
			void findPhysicalDevice(std::shared_ptr<Window::SurfaceProviders::Vulkan> surfaceProvider);
			void createDevice();
	};
}