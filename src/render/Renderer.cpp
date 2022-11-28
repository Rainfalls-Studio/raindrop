#include "render/Renderer.hpp"

namespace rnd::render{
	void Renderer::init(window::Window &window){
		PROFILE_FUNCTION();
		RND_LOG("initialization of a renderer");

		allocCallback.init();

		createInstance(window);
		createPhysicalDevice();
		createLogicalDevice();
		createRenderer();
	}

	void Renderer::createInstance(window::Window &window){
		PROFILE_FUNCTION();

		vulkan::InstanceBuilder builder;
		builder.setWindow(window.getWindowHandle());
		builder.enableValidationLayers(true);

		instance.init(builder);
	}

	void Renderer::createPhysicalDevice(){
		PROFILE_FUNCTION();

		vulkan::PhysicalDeviceBuidler builder;
		builder.setInstance(&instance);

		builder.requireFamily(vulkan::FAMILY_GRAPHIC);
		builder.requireFamily(vulkan::FAMILY_PRESENT);

		physicalDevice.init(builder);
	}

	void Renderer::createLogicalDevice(){
		PROFILE_FUNCTION();

		vulkan::LogicalDeviceBuilder builder;

		builder.setPhysicalDevice(&physicalDevice);
		builder.requireQueue(vulkan::FAMILY_GRAPHIC, 1);
		builder.requireQueue(vulkan::FAMILY_PRESENT, 1);

		builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		logicalDevice.init(builder);
	}

	void Renderer::createRenderer(){
		PROFILE_FUNCTION();
		vulkan::renderer::BasicRendererBuilder builder;

		builder.setDevice(&logicalDevice);
		builder.setExtent({1080, 720});
		builder.setBufferCount(2);
		builder.setRefreshMode(vulkan::SwapChainRefreshMode::VSync);

		renderer.init(builder);
	}

	void Renderer::shutdown(){
		PROFILE_FUNCTION();
		RND_LOG("shutdown of a renderer");
		
		instance.shutdown();
		physicalDevice.shutdown();
		logicalDevice.shutdown();
		renderer.shutdown();
	}

	void Renderer::render(layers::LayerStack& stack, const FrameData& data){
		PROFILE_FUNCTION();

		VkCommandBuffer commandBuffer = renderer.beginFrame();

		if (commandBuffer){
			stack.offscreenRender(data);
			renderer.beginSwapChainRenderPass();
			stack.render(data);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}
}