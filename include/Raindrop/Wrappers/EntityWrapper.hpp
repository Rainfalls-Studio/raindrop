#ifndef __RAINDROP_WRAPPERS_ENTITY_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_ENTITY_WRAPPER_HPP__

#include <Wrappers/common.hpp>

namespace Raindrop::Wrappers{
	class EntityWrapper{
		friend class SceneWrapper;
		public:
			EntityWrapper();
			~EntityWrapper();

			EntityWrapper(const EntityWrapper& other);
			EntityWrapper& operator=(const EntityWrapper& other);

			bool exists() const;
			bool hasComponent(const char* name) const;
			bool hasComponent(ComponentID ID) const;

			template<typename T>
			bool hasComponent() const{
				return hasComponent(typeid(T).name());
			}

			template<typename T>
			T& getComponent(ComponentID ID){
				return *static_cast<T*>(_scene->getComponent(_ID, ID));
			}

			template<typename T>
			T& getComponent(const char* name){
				return *static_cast<T*>(_scene->getComponent(_ID, _scene->getComponentID(name)));
			}

			template<typename T>
			T& getComponent(){
				return getComponent<T>(typeid(T).name());
			}

			bool operator==(EntityID ID) const;
			bool operator==(const EntityWrapper& other) const;

			bool operator!=(EntityID ID) const;
			bool operator!=(const EntityWrapper& other) const;

		private:
			EntityWrapper(Scene* scene, EntityID ID);

			Scene* _scene;
			EntityID _ID;
	};
}

#endif