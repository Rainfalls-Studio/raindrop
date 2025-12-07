#pragma once

#include <list>
#include "common.hpp"
#include "../Version.hpp"
#include "../Operator.hpp"

namespace Raindrop::Modules{
    class Dependency{
        public:
            inline Dependency(
                Name dependency,
                Version version = Version{},
                CompareOperator op = CompareOperator::EQUAL
            ) :
                _dependency(dependency),
                _version{version},
                _op{op}
            {}

            Dependency(const Dependency& other) = default;
            Dependency(Dependency&& other) = default;

            Dependency& operator=(const Dependency& other) = default;
            Dependency& operator=(Dependency&& other) = default;

            inline const Name& get() const noexcept{
                return _dependency;
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
    };

    using DependencyList = std::list<Dependency>;
}