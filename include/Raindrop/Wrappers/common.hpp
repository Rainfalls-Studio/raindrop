#ifndef __RAINDROP_WRAPPERS_HPP__
#define __RAINDROP_WRAPPERS_HPP__

#include <common.hpp>
#include <Core/Scene/Scene.hpp>

namespace Raindrop::Wrappers{
	using Raindrop::Core::Scene::Scene;
	using Raindrop::Core::Scene::EntityID;
	using Raindrop::Core::Scene::ComponentID;
	using Raindrop::Core::Scene::Signature;
	using Raindrop::Core::Scene::System;
	using Raindrop::Core::Scene::INVALID_ENTITY;
	using Raindrop::Core::Scene::INVALID_COMPONENT;
	
	typedef std::weak_ptr<Scene> ScenePtr;

	class EntityWrapper;
	class SceneWrapper;
	class SystemBase;
}

#endif