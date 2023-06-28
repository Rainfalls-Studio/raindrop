#ifndef __RAINDROP_GRAPHICS_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_CONTEXT_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/TransfertCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsCommandPool.hpp>

namespace Raindrop::Graphics{
	struct GraphicsContext{
		GraphicsContext(Core::EngineContext& context, Core::Scene::Scene& scene);
		~GraphicsContext();

		GraphicsContext(const GraphicsContext &) = delete;
		GraphicsContext& operator=(const GraphicsContext &) = delete;

		Core::EngineContext* operator->() const{
			return &context;
		}
		
		Core::EngineContext& context;
		Core::Scene::Scene& scene;

		VkAllocationCallbacks* allocationCallbacks = nullptr;

		Window window;
		Instance instance;
		Device device;
		Swapchain swapchain;

		GraphicsCommandPool graphicsCommandPool;
		TransfertCommandPool transfertCommandPool;

		uint32_t graphicsFamily;
		uint32_t transfertFamily;
		uint32_t presentFamily;

		VkQueue graphicsQueue;
		VkQueue transfertQueue;
		VkQueue presentQueue;
	};
}

#endif