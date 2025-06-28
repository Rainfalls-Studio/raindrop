#pragma once

#include <queue>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include "ResourceDefinition.hpp"

namespace Raindrop::Graphics::ResourceManager{
    using ResourceID = uint32_t;
    static constexpr ResourceID INVALID_RESOURCE_ID = ~(ResourceID)(0);

    class ResourceManager{
        public:
            ResourceManager();
            ~ResourceManager();

            ResourceID insertResource(const std::string& name, const std::type_index& type, std::shared_ptr<ResourceDefinition> resource);

            template<typename T, typename... Args>
            ResourceID insertResource(const std::string& name, Args... args);

            template<typename T>
            ResourceID insertResource(const std::string& name);

            std::shared_ptr<ResourceDefinition> getResourceFromID(ResourceID ID);

            template<typename T>
            std::shared_ptr<T> getResourceFromID(ResourceID ID);

            std::shared_ptr<ResourceDefinition> getResourceFromName(const std::string& name);

            template<typename T>
            std::shared_ptr<T> getResourceFromName(const std::string& name);


        private:
            std::unordered_map<std::string, ResourceID> _nameToID;

            struct DefinitionInfo{
                std::shared_ptr<ResourceDefinition> definition;
                std::string name;
                std::type_index type;
            };

            std::vector<DefinitionInfo> _resources;
            std::queue<ResourceID> _freeIDs;
            ResourceID _nextID;
            
            template<typename T>
            static constexpr inline void checkResourceType();
            
    };
}

#include "inl/ResourceManager.inl"