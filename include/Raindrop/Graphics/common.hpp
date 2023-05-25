#ifndef __RAINDROP_GRAPHICS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_COMMON_HPP__

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>

#include <Raindrop/common.hpp>
#include <Raindrop/Graphics/exception.hpp>
#include <Raindrop/Core/tools.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>

namespace Raindrop::Graphics{
	class Instance;
	class PhysicalDevice;
	class Device;
	class DeviceManager;
	class Swapchain;
	class Renderer;
	class Window;
	class Shader;
}

#endif