#ifndef __RAINDROP_MANAGERS_SCENE_MANAGER_HPP__
#define __RAINDROP_MANAGERS_SCENE_MANAGER_HPP__

#include <Managers/common.hpp>

namespace Raindrop::Managers{
	class RAINDROP_API SceneManager{
		public:
			SceneManager();
			~SceneManager();

			ScenePtr createScene(usize capacity);
			void destroyScene(ScenePtr scene);

		private:
			struct SceneData{
				std::shared_ptr<Scene> _scene;

				bool operator==(const ScenePtr& other){
					return _scene == other.lock();
				}
			};

			std::list<SceneData> _scenes;
	};
}

#endif