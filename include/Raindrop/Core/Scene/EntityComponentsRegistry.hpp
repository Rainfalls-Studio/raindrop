#ifndef __RAINDROP_CORE_SCENE_ENTITY_COMPONENTS_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_COMPONENTS_REGISTRY_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class EntityComponentsRegistry{
		public:
			EntityComponentsRegistry(uint32_t entityCount, uint32_t componentCount);
			~EntityComponentsRegistry();

			ComponentHandleID& get(EntityID entity, ComponentID component);
			const ComponentHandleID& get(EntityID entity, ComponentID component) const;
			void set(EntityID entity, ComponentID component, ComponentHandleID handle);

			uint32_t componentCount() const;
			uint32_t entityCount() const;

		private:
			std::vector<std::vector<ComponentHandleID>> _ids;

	};
}

#endif