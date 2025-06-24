#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <memory>

namespace Raindrop::Input{
    class StorageDefinition{
        public:
            using AccessorFnc = std::function<void*(void)>;

            StorageDefinition() = default;
            ~StorageDefinition() = default;

            template<typename T>
            void declare(const std::string& name, std::function<T&(void)> accessor);

            void declare(const std::string& name, std::type_index type, AccessorFnc accessor);

            void* access(const std::string& member);

            template<typename T>
            T& access(const std::string& member);

        private:
            struct MemberDefinition{
                std::type_index type;
                AccessorFnc accessor;
            };

            std::unordered_map<std::string, MemberDefinition> _members;
    };

    class StorageGroup{
        public:
            virtual ~StorageGroup() = default;

            StorageDefinition definition;

            template<typename T>
            T& access(const std::string& member);

            void* access(const std::string& member);
    };

    class Storage{
        public:
            Storage();

            void declareStorage(const std::string& name, std::unique_ptr<StorageGroup> group);

            StorageGroup& getGroup(const std::string& name);

            template<typename T>
            T& getGroup(const std::string& name);


            void* access(const std::string& name);

            template<typename T>
            T& access(const std::string& name);

        private:
            std::unordered_map<std::string, std::unique_ptr<StorageGroup>> _groups;
    };
}

#include "inl/Storage.inl"