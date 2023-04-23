#ifndef __RAINDROP_MANAGERS_COMMON_HPP__
#define __RAINDROP_MANAGERS_COMMON_HPP__

#include <common.hpp>
#include <Core/Scene/Scene.hpp>

namespace Raindrop::Managers{
	typedef Core::Scene::Scene* ScenePtr;
	typedef void* AssetPtr;

	static constexpr ScenePtr INVALID_SCENE_PTR = nullptr;
}

#endif