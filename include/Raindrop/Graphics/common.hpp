#ifndef __RAINDROP_GRAPHICS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_COMMON_HPP__

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <SDL2/SDL.h>

#include <Raindrop/common.hpp>
#include <Raindrop/Graphics/exception.hpp>
#include <Raindrop/Core/tools.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/EngineContext.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Graphics{
	class GraphicsContext;
	class Renderer;
	class Instance;
	class PhysicalDevice;
	class PhysicalDeviceManager;
	class Device;
	class DeviceManager;
	class Swapchain;
	class Renderer;
	class Window;
	class Shader;
	class Model;
	class GraphicsPipeline;
	class ImGUI;
	class Buffer;
	class GraphicsCommandPool;
	class TransfertCommandPool;
	class WorldFramebuffer;
	class OverlayFramebuffer;

	struct Vertex{
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};
	};
	
	struct SwapchainSupport{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		bool supported;
	};
}

#endif