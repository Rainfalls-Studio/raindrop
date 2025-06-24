#include "Raindrop/Input/Manager.hpp"

namespace Raindrop::Input{

    Manager::Manager(Engine& engine, std::shared_ptr<Event::Manager> events) : 
        _engine{engine},
        _events{events}
    {
        
    }

    void Manager::declareStorage(const std::string& name, std::unique_ptr<StorageGroup> group){
        _storage.declareStorage(name, std::move(group));
    }

    void* Manager::access(const std::string& name){
        return _storage.access(name);
    }

    StorageGroup& Manager::getGroup(const std::string& name){
        return _storage.getGroup(name);
    }
}