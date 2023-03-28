#ifndef __VULKANMODULE_CORE_HPP__
#define __VULKANMODULE_CORE_HPP__

#include <Raindrop.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <SDL2/SDL.h>

using namespace Raindrop;
using namespace Raindrop::Core::Memory;
namespace gfx = Graphics;
namespace maths = Core::Maths;
namespace dbg = Core::Debug;
namespace scene = Core::Scene;

SDL_Window* getInitWindow();

#define INVALID_FAMILY (uint32)-1

#endif