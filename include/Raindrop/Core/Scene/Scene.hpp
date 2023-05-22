#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Scene{
		public:
			Scene();
			~Scene();

			Entity& root();

			void tree();

		private:
			std::unique_ptr<Entity> _root;

	};
}

#endif