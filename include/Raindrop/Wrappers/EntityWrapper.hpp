#ifndef __RAINDROP_WRAPPERS_ENTITY_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_ENTITY_WRAPPER_HPP__

#include <Wrappers/common.hpp>

namespace Raindrop::Wrappers{
	class RAINDROP_API EntityWrapper{
		friend class SceneWrapper;
		public:
			EntityWrapper(ScenePtr scene, EntityID ID);
			EntityWrapper();
			~EntityWrapper();

			EntityWrapper(const EntityWrapper& other);
			EntityWrapper& operator=(const EntityWrapper& other);
			
			bool hasComponent(const char* name) const;
			bool hasComponent(ComponentID ID) const;

			template<typename T>
			bool hasComponent() const{
				return hasComponent(typeid(T).name());
			}

			template<typename T>
			T& getComponent(ComponentID ID){
				return *static_cast<T*>(_scene.lock()->getComponent(_ID, ID));
			}

			template<typename T>
			T& getComponent(const char* name){
				return *static_cast<T*>(_scene.lock()->getComponent(_ID, _scene.lock()->getComponentID(name)));
			}

			template<typename T>
			T& getComponent(){
				return getComponent<T>(typeid(T).name());
			}

			template<typename T>
			T& addComponent(T&& t = T{}, const char* name = typeid(T).name()){
				ComponentID componentID = _scene.lock()->getComponentID(name);
				_scene.lock()->addComponent(_ID, componentID, static_cast<void*>(&t));
				return getComponent<T>(componentID);
			}

			bool operator==(EntityID ID) const;
			bool operator==(const EntityWrapper& other) const;

			bool operator!=(EntityID ID) const;
			bool operator!=(const EntityWrapper& other) const;

		private:

			ScenePtr _scene;
			EntityID _ID;
	};
}

#endif