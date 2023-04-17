#ifndef __RAINDROP_CORE_SCENE_COMMON_HPP__
#define __RAINDROP_CORE_SCENE_COMMON_HPP__

#include <common.hpp>

namespace Raindrop::Core::Scene{
	// Up to 4,3B entities
	typedef ID32 EntityID;
	typedef ID32 ComponentID;
	static constexpr EntityID INVALID_ENTITY = INVALID_ID32;
	static constexpr ComponentID INVALID_COMPONENT = INVALID_ID32;

	// TODO: RAI-87
	static constexpr uint32 MAX_COMPONENT_COUNT = 64;
}

#endif