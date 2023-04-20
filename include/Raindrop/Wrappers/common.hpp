#ifndef __RAINDROP_WRAPPERS_HPP__
#define __RAINDROP_WRAPPERS_HPP__

#include <common.hpp>
#include <Core/Scene/Scene.hpp>
#include <Core/Memory/allocators/Allocator.hpp>

namespace Raindrop::Wrappers{
	using Raindrop::Core::Scene::Scene;
	using Raindrop::Core::Scene::EntityID;
	using Raindrop::Core::Scene::ComponentID;
	using Raindrop::Core::Scene::Signature;
	using Raindrop::Core::Scene::INVALID_ENTITY;
	using Raindrop::Core::Scene::INVALID_COMPONENT;

	class EntityWrapper;
	class SceneWrapper;
	class SystemWrapper;
}

#endif