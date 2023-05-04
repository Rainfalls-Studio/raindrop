#ifndef __RAINDROP_MANAGERS_COMMON_HPP__
#define __RAINDROP_MANAGERS_COMMON_HPP__

#include <common.hpp>
#include <Core/Scene/Scene.hpp>

namespace Raindrop::Managers{
	class Asset;
	class AssetFactory;

	using Raindrop::Core::Scene::Scene;
	using Raindrop::Core::Scene::EntityID;


	using AssetPtr = std::weak_ptr<Asset>;
	using ScenePtr = std::weak_ptr<Scene>;
}

#endif