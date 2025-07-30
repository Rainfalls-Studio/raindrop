#pragma once

#include <cstdint>
#include <entt/entt.hpp>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Layers{
    using LayerHandle = uint32_t;
    static constexpr LayerHandle INVALID_LAYER_HANDLE = static_cast<LayerHandle>(0u);

    class Layer;

    class Manager{
        public:
            Manager(Engine& engine);
            ~Manager();

            Layer createLayer();
            Layer getLayer(LayerHandle handle);

            template<typename... Modules>
            inline decltype(auto) getModules(LayerHandle handle){
                return _layers.get<Modules...>(handle);
            }

            template<typename Module, typename... Args>
            inline decltype(auto) emplaceModule(LayerHandle handle, Args&&... args){
                return _layers.emplace<Module, Args...>(handle, std::forward<Args>(args)...);
            }

            template<typename Module, typename... Others>
            inline decltype(auto) view(){
                return _layers.view<Module, Others...>();
            }

            template<typename Module>
            inline decltype(auto) group(){
                return _layers.group<Module>();
            }

            entt::basic_registry<LayerHandle>& getRegistry() noexcept;

            bool isValid(LayerHandle handle) const;

        private:
            Engine& _engine;
            entt::basic_registry<LayerHandle> _layers;
    };
}