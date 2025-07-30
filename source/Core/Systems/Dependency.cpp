#include "Raindrop/Core/Systems/Dependency.hpp"

namespace Raindrop::Systems{

    Dependency::Dependency(std::string dependency, bool optional) : 
        _dependency{dependency},
        _optional{optional}
    {}

    Dependency::Dependency(const Dependency& other) : 
        _dependency{other._dependency},
        _optional{other._optional}
    {}

    Dependency& Dependency::operator=(const Dependency& other){
        _dependency = other._dependency;
        _optional = other._optional;
        return *this;
    }

    Dependency& Dependency::operator=(Dependency&& other){
        std::swap(_dependency, other._dependency);
        std::swap(_optional, other._optional);
        return *this;
    }

    const std::string& Dependency::getDependency() const noexcept{
        return _dependency;
    }

    bool Dependency::isOptional() const noexcept{
        return _optional;
    }
}