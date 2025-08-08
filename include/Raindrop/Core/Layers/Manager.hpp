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

            void shutdown();

            Layer createLayer();
            Layer getLayer(LayerHandle handle);

            template<typename... Traits>
            inline decltype(auto) getTraits(LayerHandle handle){
                return _layers.get<Traits...>(handle);
            }

            template<typename Trait, typename... Args>
            inline decltype(auto) emplaceTrait(LayerHandle handle, Args&&... args){
                return _layers.emplace<Trait, Args...>(handle, std::forward<Args>(args)...);
            }

            template<typename Trait, typename... Others>
            inline decltype(auto) view(){
                return _layers.view<Trait, Others...>();
            }

            template<typename Trait>
            inline decltype(auto) group(){
                return _layers.group<Trait>();
            }

            entt::basic_registry<LayerHandle>& getRegistry() noexcept;

            bool isValid(LayerHandle handle) const;

        private:
            Engine& _engine;
            entt::basic_registry<LayerHandle> _layers;
    };
}