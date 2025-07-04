#pragma once

#include <type_traits>
#include <memory>
#include <typeindex>
#include <vector>
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class CommandList{
        public:
            enum class Level{
                PRIMARY = 0,
                SECONDARY = 1
            };

            class Domain{
                public:
                    virtual ~Domain() = default;
            };

            virtual ~CommandList() = default;

            template<typename T>
            std::shared_ptr<T> getDomain(){
                static_assert(std::is_base_of<Domain, T>::value, "T must be a subclass of Domain");
                return std::static_pointer_cast<T>(getDomain(typeid(T)));
            }

            virtual std::shared_ptr<Domain> getDomain(const std::type_index& type) const = 0;

            virtual void execute(const std::vector<std::shared_ptr<CommandList>>& commands) = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}