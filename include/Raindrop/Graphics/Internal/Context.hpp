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
			~Context();

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
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<Window> _window;
			std::unique_ptr<Instance> _instance;
			std::unique_ptr<PhysicalDevice> _physicalDevice;
			std::unique_ptr<Device> _device;
			std::unique_ptr<QueueHandler> _queueHandler;
			std::unique_ptr<CommandPools> _commandPools;
			std::unique_ptr<Swapchain> _swapchain;

		
	};
}

#include <Raindrop/Graphics/Context.hpp>

#endif