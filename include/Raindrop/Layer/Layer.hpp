#pragma once

#include "Manager.hpp"

namespace Raindrop::Layer{
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

            template<typename... Modules>
            inline decltype(auto) getModules(){
                return _manager->getModules<Modules...>(getHandle());
            }

            template<typename Modules, typename... Args>
            inline decltype(auto) emplaceModule(Args&&... args){
                return _manager->emplaceModule<Modules, Args...>(getHandle(), std::forward<Args>(args)...);
            }

        private:
            Manager* _manager;
            LayerHandle _handle;
    };
}