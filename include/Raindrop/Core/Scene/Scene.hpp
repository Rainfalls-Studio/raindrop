#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Scene{
		public:
			Scene(Registry::RegistryManager& manager);
			~Scene();

			Entity& root();
			Registry::Registry& registry() const;

			void tree();

		private:
			Registry::Registry& _registry;
			std::unique_ptr<Entity> _root;

	};
}

#endif