#pragma once

#include <string>

namespace Raindrop::Systems{
    class Dependency{
        public:
            Dependency(std::string dependency, bool optional = false);
            Dependency(const Dependency& other);

            Dependency& operator=(const Dependency& other);
            Dependency& operator=(Dependency&& other);

            const std::string& getDependency() const noexcept;
            bool isOptional() const noexcept;

        private:
            std::string _dependency;
            bool _optional;
    };
}