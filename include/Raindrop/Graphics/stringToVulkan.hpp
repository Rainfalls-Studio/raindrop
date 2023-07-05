#ifndef __RAINDROP_GRAPHICS_STRING_TO_VULKAN_HPP__
#define __RAINDROP_GRAPHICS_STRING_TO_VULKAN_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	uint32_t strToVkEnum(const std::string& str, uint32_t d);

	template<typename T>
	T strToVkEnumT(const std::string& str, T d){
		return static_cast<T>(strToVkEnum(str, static_cast<uint32_t>(d)));
	}
}

#endif