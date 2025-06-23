#pragma once

#include "Raindrop/Core/Internal/Config/Tree.hpp"
#include "Node.hpp"
#include "Tag.hpp"

namespace Raindrop::Core::Config{
    class Tree{
        public:
            inline Tree();
            inline Tree(std::shared_ptr<Core::Internal::Config::Tree> impl);
            inline ~Tree() = default;

            static Tree Create();

            inline bool isValid() const noexcept;

            inline void parseFile(const std::string& path);

            inline void parseStr(const std::string& string);

            inline Node getRoot() noexcept;

            inline const Node getRoot() const noexcept;

			inline Node operator[](const Tag& tag);	
			inline const Node operator[](const Tag& tag) const;

			inline Node getNode(const Tag& tag);
			inline const Node getNode(const Tag& tag) const;
            
            inline std::shared_ptr<Core::Internal::Config::Tree> get();

        private:
            std::shared_ptr<Core::Internal::Config::Tree> _impl;

            Core::Internal::Config::Tree& assertGet();
            const Core::Internal::Config::Tree& assertGet() const;
    };
}

#include "inl/Tree.inl"