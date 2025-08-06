#pragma once

#include "Manager.hpp"

namespace Raindrop::Layers{
    class Layer{
        public:
            Layer();
            Layer(Manager* manager, LayerHandle handle);

            Layer(const Layer& other);
            Layer(Layer&& other);

            Layer& operator=(Layer&& other);
            Layer& operator=(const Layer& other);

            bool isValid() const noexcept;

            inline operator bool() const noexcept{
                return isValid();
            }

            LayerHandle getHandle() const noexcept;
            Manager* getManager() const noexcept;

            template<typename... Traits>
            inline decltype(auto) getTraits(){
                return _manager->getTraits<Traits...>(getHandle());
            }

            template<typename Traits, typename... Args>
            inline decltype(auto) emplaceTrait(Args&&... args){
                return _manager->emplaceTrait<Traits, Args...>(getHandle(), std::forward<Args>(args)...);
            }

        private:
            Manager* _manager;
            LayerHandle _handle;
    };
}