#pragma once

#include <string>
#include <list>

namespace Raindrop::Modules{
    class Dependency{
        public:
            inline Dependency(std::string dependency, bool optional = false) : _dependency(dependency), _optional(optional){}
            
            Dependency(const Dependency& other) = default;
            Dependency(Dependency&& other) = default;

            Dependency& operator=(const Dependency& other) = default;
            Dependency& operator=(Dependency&& other) = default;

            inline const std::string& get() const noexcept{
                return _dependency;
            }

            inline bool optional() const noexcept{
                return _optional;
            }

            inline bool operator==(const Dependency& other) const noexcept{
                return other._dependency == _dependency;
            }

        private:
            std::string _dependency;
            bool _optional;
    };

    using DependencyList = std::list<Dependency>;
}