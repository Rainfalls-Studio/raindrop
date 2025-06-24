#include "Raindrop/Input/Storage.hpp"
#include <stdexcept>

namespace Raindrop::Input{
    void StorageDefinition::declare(const std::string& name, std::type_index type, AccessorFnc accessor){
        _members.emplace(
            name,
            MemberDefinition{
                type,
                accessor
        });
    }

    void* StorageDefinition::access(const std::string& member){
        auto it = _members.find(member);

        if (it == _members.end()){
            throw std::runtime_error("Member not found: " + member);
        }

        return it->second.accessor();
    }

    void* StorageGroup::access(const std::string& member){
        return definition.access(member);
    }


    Storage::Storage(){

    }

    void Storage::declareStorage(const std::string& name, std::unique_ptr<StorageGroup> group){
        _groups.emplace(name, std::move(group));
    }

    StorageGroup& Storage::getGroup(const std::string& name){
        auto it = _groups.find(name);
        if (it == _groups.end()){
            throw std::runtime_error("Storage group not found: " + name);
        }

        return *it->second;
    }

    void* Storage::access(const std::string& name){
        auto separator = name.find_first_of(".");

        if (separator == std::string::npos){
            throw std::runtime_error("Invalid storage name: " + name);
        }

        std::string storageName = name.substr(0, separator);
        StorageGroup& group = getGroup(storageName);

        return group.access(name.substr(separator+1));
    }
}