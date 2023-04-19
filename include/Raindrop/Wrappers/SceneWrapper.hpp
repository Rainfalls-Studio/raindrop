#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Managers/common.hpp>

namespace Raindrop::Wrappers{
	class SceneWrapper{
		public:
			SceneWrapper(const SceneWrapper& other);
			SceneWrapper(Managers::ScenePtr scene);
			~SceneWrapper();

			SceneWrapper& operator=(const SceneWrapper& other);

			EntityWrapper createEntity();
			void destroyEntity(EntityWrapper entity);
			usize capacity();

			ComponentID getComponentID(const char* name) const;

			template<typename T>
			ComponentID getComponentID() const{
				return getComponentID(typeid(T).name());
			}

			Managers::ScenePtr getScenePtr() const;

		private:
			Managers::ScenePtr _scene;
	};
}

#endif