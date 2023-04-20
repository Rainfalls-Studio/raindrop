#ifndef __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SYSTEM_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>
#include <Managers/common.hpp>

namespace Raindrop::Wrappers{
	class SystemWrapper : private Core::Scene::System{
		public:
			SystemWrapper(Core::Memory::Allocator& allocator = RAINDROP_default_allocator) : Core::Scene::System(allocator){}
			virtual ~SystemWrapper() override = default;

		protected:
			virtual void OnEntityCreated(EntityWrapper entity){}
			virtual void OnEntityDestroyed(EntityWrapper entity){}

		private:
			Managers::ScenePtr _scene;

			virtual void onEntityCreated(ID32 entity) override final{
				OnEntityCreated(EntityWrapper(_scene, entity));
			}

			virtual void onEntityDestroyed(ID32 entity) override final{
				OnEntityDestroyed(EntityWrapper(_scene, entity));
			}
	};
}

#endif