#ifndef __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	class SystemBase : public System{
		friend class SceneWrapper;
		public:
			SystemBase() : System(){}
			virtual ~SystemBase() override = default;

		protected:
			class Iterator{
				public:
					using value_type = std::list<EntityID>::const_iterator;
					using pointer = std::list<EntityID>::const_iterator*;
					using reference = std::list<EntityID>::const_iterator&;
					using iterator_category = std::forward_iterator_tag;
					using difference_type = std::ptrdiff_t;

					Iterator(value_type val, ScenePtr scene) : _val(val), _scene{scene}{}

					EntityWrapper operator*() const {
						return EntityWrapper(_scene, *_val);
					}

					Iterator& operator++() {
						++_val;
						return *this;
					}

					Iterator operator++(int) {
						Iterator tmp(*this);
						++_val;
						return tmp;
					}

					bool operator==(const Iterator& other) const {
						return _val == other._val;
					}

					bool operator!=(const Iterator& other) const {
						return !(*this == other);
					}

				private:
					ScenePtr _scene;
					value_type _val;
			};
		
			class EntityList{
				public:
					EntityList(std::list<EntityID>& entities, ScenePtr scene) : _entities{entities}, _scene{scene}{}

					Iterator begin(){
						return Iterator(_entities.begin(), _scene);
					}

					Iterator end(){
						return Iterator(_entities.end(), _scene);
					}

				private:
					ScenePtr _scene;
					std::list<EntityID>& _entities;
			};

			virtual void OnEntityCreate(EntityWrapper entity){}
			virtual void OnEntityDestroy(EntityWrapper entity){}

			EntityList entities(){
				return EntityList(_entities, _scene);
			}

		private:
			ScenePtr _scene;

			virtual void onEntityCreated(EntityID id) override final{
				OnEntityCreate(EntityWrapper(_scene, id));
			}

			virtual void onEntityDestroyed(EntityID id) override final{
				OnEntityDestroy(EntityWrapper(_scene, id));
			}

			System* getSystem(){
				return static_cast<System*>(this);
			}
	};
}

#endif