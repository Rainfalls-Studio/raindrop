#ifndef __RAINDROP_GRAPHICS_UTILS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_UTILS_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Utils/common.hpp>
#include <Raindrop/Graphics/Utils/FormatUtilities.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>

namespace Raindrop::Graphics::Utils{
	struct Context{
		Graphics::Context& graphics;

		spdlog::logger logger;
		FormatUtilities formats;
		StringToVulkan stringToVulkan;

		Context(Graphics::Context& graphics);
	};
}

#include <Raindrop/Graphics/Context.hpp>

#endif