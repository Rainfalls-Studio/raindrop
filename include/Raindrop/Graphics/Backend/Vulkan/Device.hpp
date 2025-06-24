#pragma once

#include <VkBootstrap.h>
#include "../Device.hpp"
#include "Context.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
	class Device : public Backend::Device{
		public:
			Device(Engine& engine);

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

			void createLogger();
			void createInstance();
			void findPhysicalDevice();
			void createDevice();
	};
}