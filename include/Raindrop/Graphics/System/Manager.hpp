#pragma once

#include "IRenderSystem.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>


namespace Raindrop::Graphics::System{
    // TODO: base class with Raindrop::System::Manager
    class Manager{
        public:
            Manager();
            ~Manager();

            void registerSystem(const std::type_index& type, std::shared_ptr<IRenderSystem>&& system);
            void unregisterSystem(const std::type_index& type);

            std::shared_ptr<IRenderSystem> getSystem(const std::type_index& type) const;

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

            void foreach(const std::function<void(IRenderSystem&)>& callback);

            void initializeSystems(GraphicsEngine& engine);
            void shutdownSystems();

        private:
            using RenderSystemID = size_t;
            static constexpr RenderSystemID INVALID_RENDER_SYSTEM_ID = ~static_cast<RenderSystemID>(0);

            void populateDependents();
            void setupInitList();
            RenderSystemID getSystemIDFromType(const std::type_index& type);
            bool isSystemTreeCyclic();

            struct SystemInfo{
                std::shared_ptr<IRenderSystem> system = {};
                std::vector<RenderSystemID> dependents = {};
                std::vector<RenderSystemID> dependencies = {};
                RenderSystemID id = INVALID_RENDER_SYSTEM_ID;
            };

            std::vector<SystemInfo> _infos;
            std::vector<RenderSystemID> _initList;
            std::unordered_map<std::type_index, RenderSystemID> _systems;

            template<typename T>
            static inline constexpr void assertDerivedBase() noexcept{
                static_assert(std::is_base_of<IRenderSystem, T>::value, "System must be derived from ISystem");
            }
    };
}