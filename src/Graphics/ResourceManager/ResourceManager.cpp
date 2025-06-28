#include "Raindrop/Graphics/ResourceManager/ResourceManager.hpp"
#include <cassert>
#include <stdexcept>
#include <typeindex>

namespace Raindrop::Graphics::ResourceManager{
    ResourceManager::ResourceManager(){}

    ResourceID ResourceManager::insertResource(const std::string& name, const std::type_index& type, std::shared_ptr<ResourceDefinition> resource){
        if (_nameToID.count(name) == 1){
            throw std::runtime_error("Resource with name " + name + " already exists");
        }

        DefinitionInfo info = {
            .definition = resource,
            .name = name,
            .type = type
        };

        ResourceID ID = INVALID_RESOURCE_ID;
        if (_freeIDs.empty()){
            ID = _nextID++;
            _nameToID[name] = ID;
            _resources.push_back(info);

        } else {
            ID = _freeIDs.front();
            _freeIDs.pop();

            _nameToID[name] = ID;
            _resources[ID] = info;
        }

        return ID;
    }

    std::shared_ptr<ResourceDefinition> ResourceManager::getResourceFromID(ResourceID ID){
        return _resources[ID].definition;
    }

    std::shared_ptr<ResourceDefinition> ResourceManager::getResourceFromName(const std::string& name){
        return getResourceFromID(_nameToID[name]);
    }
}