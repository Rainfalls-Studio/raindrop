#pragma once

#include <list>
#include "common.hpp"
#include "../Version.hpp"

namespace Raindrop{
    class Dependency{
        public:
            inline Dependency(
                Name dependency,
                VersionConstraint constraint = {}
            ) :
                _dependency(dependency),
                _constraint{std::move(constraint)}
            {}

            
            Dependency(const Dependency& other) : 
                _dependency(other._dependency),
                _constraint(other._constraint)
            {}
            
            Dependency& operator=(const Dependency& other) = delete;

            Dependency(Dependency&& other) = default;
            Dependency& operator=(Dependency&& other) = default;

            ~Dependency() = default;

            inline const Name& get() const noexcept{
                return _dependency;
            }

            inline const VersionConstraint& constraint() const noexcept{
                return _constraint;
            }

            inline bool operator==(const Dependency& other) const noexcept{
                return other._dependency == _dependency;
            }

        private:
            Name _dependency;
            VersionConstraint _constraint;
    };

    using DependencyList = std::list<Dependency>;
}