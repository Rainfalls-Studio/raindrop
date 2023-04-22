#ifndef __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	class SystemBase : private System{
		friend class SceneWrapper;
		public:
			SystemBase(Core::Memory::Allocator& allocator = RAINDROP_default_allocator) : System(allocator){}
			virtual ~SystemBase() override = default;

		protected:
			virtual void OnEntityCreate(EntityWrapper entity){};
			virtual void OnEntityDestroy(EntityWrapper entity){};

			const Core::Memory::DoublyLinkedList<EntityID> getEntities() const{
				return _entities;
			}

		private:
			ScenePtr _scene;

			virtual void onEntityCreated(EntityID id) override final{
				OnEntityCreate(EntityWrapper(_scene, id));
			}

			virtual void onEntityDestroyed(EntityID id) override final{
				OnEntityDestroy(EntityWrapper(_scene, id));
			}
	};
}

#endif