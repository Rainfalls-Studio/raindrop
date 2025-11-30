#pragma once

#include <string>
#include <list>
#include "common.hpp"
#include "../Version.hpp"
#include "../Operator.hpp"

namespace Raindrop::Modules{
    class Dependency{
        public:
            inline Dependency(
                bool optional,
                Name dependency,
                Version version = Version{},
                CompareOperator op = CompareOperator::EQUAL
            ) :
                _dependency(dependency),
                _version{version},
                _op{op},
                _optional(optional)
            {}

            Dependency(const Dependency& other) = default;
            Dependency(Dependency&& other) = default;

            Dependency& operator=(const Dependency& other) = default;
            Dependency& operator=(Dependency&& other) = default;

            inline const Name& get() const noexcept{
                return _dependency;
            }

            inline bool optional() const noexcept{
                return _optional;
            }

            inline const Version& version() const noexcept{
                return _version;
            }

            inline CompareOperator comparator() const noexcept{
                return _op;
            }

            inline bool versionCheck(const Version& other) const noexcept{
                return _op.getComparator<Version>()(_version, other);
            }

            inline bool operator==(const Dependency& other) const noexcept{
                return other._dependency == _dependency;
            }

        private:
            Name _dependency;
            Version _version;
            CompareOperator _op;
            bool _optional;
    };

    inline Dependency HardDependency(
        Name dependency,
        Version version = Version{},
        CompareOperator op = CompareOperator::EQUAL
    ) noexcept{
        return Dependency(false, dependency, version, op);
    }

    inline Dependency SoftDependency(
        Name dependency,
        Version version = Version{},
        CompareOperator op = CompareOperator::EQUAL
    ) noexcept{
        return Dependency(true, dependency, version, op);
    }

    using DependencyList = std::list<Dependency>;
}