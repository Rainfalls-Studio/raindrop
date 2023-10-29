#ifndef __RAINDROP_GRAPHICS_INTERNAL_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Internal/common.hpp>
#include <Raindrop/Graphics/Internal/Instance.hpp>
#include <Raindrop/Graphics/Internal/Window.hpp>
#include <Raindrop/Graphics/Internal/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Internal/Device.hpp>
#include <Raindrop/Graphics/Internal/QueueHandler.hpp>
#include <Raindrop/Graphics/Internal/Swapchain.hpp>
#include <Raindrop/Graphics/Internal/CommandPools.hpp>

namespace Raindrop::Graphics::Internal{
	class Context{
		public:
			Context(Graphics::Context& graphics);

			const VkPhysicalDeviceLimits& limits() const;

			Graphics::Context& graphics();
			spdlog::logger& logger();

			Window& window();
			Instance& instance();
			PhysicalDevice& physicalDevice();
			Device& device();
			QueueHandler& queueHandler();
			CommandPools& commandPools();
			Swapchain& swapchain();

		private:
			Graphics::Context& _graphics;
			spdlog::logger _logger;

			Window _window;
			Instance _instance;
			PhysicalDevice _physicalDevice;
			Device _device;
			QueueHandler _queueHandler;
			CommandPools _commandPools;
			Swapchain _swapchain;

		
	};
}

#include <Raindrop/Graphics/Context.hpp>

#endif