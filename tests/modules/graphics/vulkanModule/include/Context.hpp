#ifndef __VULKANMODULE_CONTEXT_HPP__
#define __VULKANMODULE_CONTEXT_HPP__

#include "core.hpp"
#include "Allocator.hpp"
#include "Swapchain.hpp"
#include "Window.hpp"
#include "device/Device.hpp"

class Context : public gfx::Context{
	public:
		Context(Device& device, GfxAllocator& allocator, Window& window);
		~Context();

		virtual bool begin() override;
		virtual void end() override;
		virtual void setClearColor(Core::Maths::vec4<float32> color) override;
		virtual void enableVSync(bool enable) override;
		virtual bool isVSyncEnabled() const override;

		VkCommandBuffer getCurrentCommandBuffer() const;
		const Swapchain& swapchain() const;
		VkRenderPass swapchainRenderPass() const;

	private:
		GfxAllocator& _allocator;
		Window& _window;
		Device& _device;
		Swapchain* _swapchain;
		VkSurfaceKHR _surface;

		VkCommandPool _commandPool;
		Array<VkCommandBuffer> _commandBuffers;
		uint32 _bufferCount;
		bool _frameStarted;

		VkExtent2D getWindowExtent();
		bool createCommandBuffers();
};

#endif