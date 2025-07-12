#pragma once

#include "ISystem.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>

namespace Raindrop::System{
    class Manager{
        public:
            Manager();
            ~Manager();

            void registerSystem(const std::type_index& type, std::shared_ptr<ISystem>&& system);
            void unregisterSystem(const std::type_index& type);

            std::shared_ptr<ISystem> getSystem(const std::type_index& type) const;

            template<typename T>
            inline void emplaceSystem(){
                registerSystem<T>(std::make_shared<T>());
            }

            template<typename T, typename... Args>
            inline void emplaceSystem(Args&&... args){
                registerSystem<T>(std::make_shared<T>(std::forward<Args>(args)...));
            }

            template<typename T>
            inline void registerSystem(std::shared_ptr<T>&& system){
                assertDerivedBase<T>();
                registerSystem(typeid(T), std::move(system));
            }

            template<typename T>
            inline void unregisterSystem(){
                assertDerivedBase<T>();
                unregisterSystem(typeid(T));
            }

            template<typename T>
            inline std::shared_ptr<T> getSystem(){
                assertDerivedBase<T>();
                return std::static_pointer_cast<T>(getSystem(typeid(T)));
            }

            void foreach(const std::function<void(ISystem&)>& callback);

            void initializeSystems(Engine& engine);
            void shutdownSystems();

        private:
            using SystemID = size_t;
            static constexpr SystemID INVALID_SYSTEM_ID = ~static_cast<SystemID>(0);

            void populateDependents();
            void setupInitList();
            SystemID getSystemIDFromType(const std::type_index& type);
            bool isSystemTreeCyclic();

            struct SystemInfo{
                std::shared_ptr<ISystem> system = {};
                std::vector<size_t> dependents = {};
                std::vector<size_t> dependencies = {};
                size_t id = INVALID_SYSTEM_ID;
            };

            std::vector<SystemInfo> _infos;
            std::vector<size_t> _initList;
            std::unordered_map<std::type_index, size_t> _systems;

            template<typename T>
            static inline constexpr void assertDerivedBase() noexcept{
                static_assert(std::is_base_of<ISystem, T>::value, "System must be derived from ISystem");
            }
    };
}