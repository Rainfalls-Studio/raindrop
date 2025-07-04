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
			
			virtual std::vector<std::shared_ptr<QueueFamily>> getFamilies() const override;

			virtual std::shared_ptr<Buffer> createBuffer(const Buffer::Description& description) override;
			virtual std::shared_ptr<BufferView> createBufferView(const BufferView::Description& description) override;
			virtual std::shared_ptr<Image> createImage(const Image::Description& description) override;
			virtual std::shared_ptr<ImageView> createImageView(const ImageView::Description& description) override;
			virtual std::shared_ptr<CommandPool> createCommandPool(const CommandPool::Description& description) override;
			virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const DescriptorSetLayout::Description& description) override;
			virtual std::shared_ptr<DescriptorPool> createDescriptorPool(const DescriptorPool::Description& description) override;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Description& description) override;
			virtual std::shared_ptr<Framebuffer> createFramebuffer(const Framebuffer::Description& description) override;
			virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline(const GraphicsPipeline::Description& description) override;
			virtual std::shared_ptr<ComputePipeline> createComputePipeline(const ComputePipeline::Description& description) override;
			virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPass::Description& description) override;
			virtual std::shared_ptr<Sampler> createSampler(const Sampler::Description& description) override;
			virtual std::shared_ptr<Shader> createShader(const Shader::Description& description) override;
			virtual std::shared_ptr<Fence> createFence(const Fence::Description& description) override;
			virtual std::shared_ptr<Semaphore> createSemaphore() override;
			virtual std::shared_ptr<TimelineSemaphore> createTimelineSemaphore(const TimelineSemaphore::Description& description) override;

			virtual void* getHandle() const noexcept override;
			virtual API getAPI() const noexcept override;

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