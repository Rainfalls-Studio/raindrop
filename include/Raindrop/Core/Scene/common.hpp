#ifndef __RAINDROP_CORE_SCENE_COMMON_HPP__
#define __RAINDROP_CORE_SCENE_COMMON_HPP__

#include <Raindrop/common.hpp>

namespace Raindrop::Core::Scene{
	using EntityID = uint32_t;
	using ComponentID = uint32_t;
	using ComponentHandleID = uint32_t;
	
	using ConstructorPtr = void(*)(void*);
	using DestructorPtr = void(*)(void*);

	static constexpr EntityID INVALID_ENTITY_ID = ~0;
	static constexpr ComponentID INVALID_COMPONENT_ID = ~0;
	static constexpr ComponentHandleID INVALID_COMPONENT_HANDLE_ID = ~0;

	class Entity;
	class Scene;
	class Component;
	class ComponentManager;
	class EntityManager;
	class ComponentRegistry;
	class EntityComponentsRegistry;
}

#endif