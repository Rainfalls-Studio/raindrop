#pragma once

#include <cstdint>
#include <entt/entt.hpp>

#include "Raindrop/Engine.hpp"

namespace Raindrop::Scene{
    using EntityHandle = uint32_t;
    static constexpr EntityHandle INVALID_ENTITY_HANDLE = ~static_cast<EntityHandle>(0);

    using BehaviorID = uint32_t;
    static constexpr BehaviorID INVALID_BEHAVIOR_ID = ~static_cast<BehaviorID>(0);
    
    using PhaseID = uint32_t;
    static constexpr PhaseID INVALID_PHASE_ID = ~static_cast<PhaseID>(0);

    class Entity;
    class IBehavior;


    class Scene{
        public:
            Scene();
            ~Scene();

            /**
             * @brief intializes the scene
             * @param engine 
             */
            void initialize(Engine& engine);

            void shutdown();

            /**
             * @brief creates a new entity
             * @return Entity 
             */
            Entity createEntity();

            /**
             * @brief Destroys given entity and invalidates it's handle
             * @param entity the entity to destroy
             */
            void destroy(Entity entity);

            /**
             * @brief Get the entity at given handle
             * @param handle the entity handle
             * @return Entity 
             */
            Entity getEntity(EntityHandle handle);

            /**
             * @brief checks if a given entity handle is invalid
             * @param handle the handle to check
             */
            bool isValid(EntityHandle handle) const;

            /**
             * @brief Get the entite's components
             * 
             * @tparam Components 
             * @param handle 
             * @return decltype(auto) 
             */
            template<typename... Components>
            inline decltype(auto) getComponent(EntityHandle handle);

            /**
             * @brief Get the entite's components
             * 
             * @tparam Components 
             * @param handle 
             * @return decltype(auto) 
             */
            template<typename... Components>
            inline decltype(auto) getComponent(EntityHandle handle) const;

            /**
             * @brief Creates a component's in place
             * 
             * @tparam Components The component to add
             * @tparam Args The component creator argument
             * @param handle the handle to add the component to
             * @param args the create arguments
             * @return decltype(auto) 
             */
            template<typename Components, typename... Args>
            inline decltype(auto) emplaceComponent(EntityHandle handle, Args&&... args);
        
            /**
             * @brief checks if any of the given components
             * 
             * @tparam Components 
             * @param handle 
             * @return true 
             * @return false 
             */
            template<typename... Components>
            inline bool hasAnyComponents(EntityHandle handle) const;

            /**
             * @brief Checks if a handle has all components
             * 
             * @tparam Components 
             * @param handle 
             * @return true 
             * @return false 
             */
            template<typename... Components>
            inline bool hasAllComponents(EntityHandle handle) const;

            /**
             * @brief Get the index of the behavior
             * 
             * @param type the behavior type
             * @return BehaviorID 
             */
            BehaviorID getBehaviorIndex(std::type_index type);

            template<typename T>
            inline BehaviorID getBehaviorIndex(){
                return getBehaviorIndex(typeid(T));
            }

            /**
             * @brief Emplace a behavior
             * 
             * @tparam T 
             * @tparam Args 
             * @param args 
             * @return BehaviorID 
             */
            template<typename T, typename... Args>
            BehaviorID emplaceBehavior(Args&&... args){
                std::shared_ptr<T> behavior = std::make_shared<T>(std::forward<Args>(args)...);
                return addBehavior(typeid(T), std::move(behavior));
            }
            
            /**
             * @brief Register a new behavior, or overwrite one
             * 
             * @param index 
             * @param behavior 
             * @return BehaviorID 
             */
            BehaviorID addBehavior(std::type_index index, std::shared_ptr<IBehavior>&& behavior);
            
            /**
             * @brief Checks if a behavior is registred
             * 
             * @param type 
             * @return true 
             * @return false 
             */
            bool hasBehavior(std::type_index type) const;

            std::shared_ptr<IBehavior> getBehavior(BehaviorID id);
            std::shared_ptr<IBehavior> getBehaviorFromType(std::type_index type);

            template<typename T>
            inline std::shared_ptr<T> getBehavior(BehaviorID id);

            template<typename T>
            inline std::shared_ptr<T> getBehaviorFromType();

            
            PhaseID createPhase(std::string_view name);
            void destroyPhase(PhaseID phase);
            std::string_view phaseName(PhaseID phase);
            void executePhase(PhaseID phase);
            void addToPhase(PhaseID phase, BehaviorID behavior);


            entt::basic_registry<EntityHandle>& registry();


        private:
            Engine* _engine;
            entt::basic_registry<EntityHandle> _registry;

            std::vector<std::shared_ptr<IBehavior>> _behaviors;
            std::unordered_map<std::type_index, BehaviorID> _typeToIndex;
            std::deque<BehaviorID> _freeBehaviorIDs;

            struct PhaseContent{

                // the list of behaviors to execute in the phase, in execution order
                std::vector<BehaviorID> behaviors;
                
                // it's name for debug and development
                std::string name;
            };

            std::vector<PhaseContent> _phases;
            std::deque<PhaseID> _freePhaseID;



            void foreachBehavior(const std::function<void(IBehavior&)>& callback);

    };
}

#include "inl/Scene.inl"