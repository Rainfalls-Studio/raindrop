#include "Raindrop/System/Manager.hpp"
#include <queue>
#include <stdexcept>

namespace Raindrop::System{
    Manager::Manager(){}
    Manager::~Manager(){}

    void Manager::registerSystem(const std::type_index& type, std::shared_ptr<ISystem>&& system){
        size_t id = _infos.size();

        _infos.push_back(SystemInfo{
            .system = std::move(system),
            .id = id
        });

        _systems[type] = id;
    }

    void Manager::unregisterSystem(const std::type_index& type){
        auto id = _systems.at(type);
        auto& info = _infos.at(id);

        for (auto& dependentId : info.dependents){
            // auto dep = dependent.lock();
            auto dependent = _infos[dependentId];
            auto depSystem = dependent.system;

            if (depSystem){
                depSystem->dependencyShutdown(type, *info.system);
            }
        }

        _systems.erase(type);
    }

    std::shared_ptr<ISystem> Manager::getSystem(const std::type_index& type) const{
        auto it = _systems.find(type);

        if (it == _systems.end()){
            return nullptr;
        }

        SystemID id = it->second;

        return _infos.at(id).system;
    }
    
    void Manager::foreach(const std::function<void(ISystem&)>& callback){
        for (auto& info : _infos){
            callback(*info.system);
        }
    }

    void Manager::initializeSystems(Engine& engine){
        populateDependents();

        if (!isSystemTreeCyclic()){
            throw std::runtime_error("The system dependency tree is cylic");
        }

        setupInitList();

        for (auto i : _initList){
            _infos[i].system->initialize(engine);
        }
    }

    void Manager::shutdownSystems(){
        // inverse traversal
        for (size_t i=_initList.size(); i>0; i--){
            _infos[i-1].system->shutdown();
        }

        _infos.clear();
        _initList.clear();
        _systems.clear();
    }

    Manager::SystemID Manager::getSystemIDFromType(const std::type_index& type){
        try{
            return _systems.at(type);
        } catch (...){
            return INVALID_SYSTEM_ID;
        }
    }

    bool Manager::isSystemTreeCyclic(){
        std::vector<size_t> inDegree(_infos.size(), 0);
        std::queue<SystemID> q;

        size_t visitedCount = 0;
        
        // Calculate in-degree for each node
        for (const auto& info : _infos) {
            inDegree[info.id] = info.dependencies.size();
        }
        
        // Enqueue nodes with in-degree 0
        for (size_t i=0; i<inDegree.size(); i++){
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
        
        while (!q.empty()) {
            SystemID id = q.front();
            q.pop();
            visitedCount++;

            auto& info = _infos[id];
            
            for (SystemID dependent : info.dependents) {
                inDegree[dependent]--;
                if (inDegree[dependent] == 0) {
                    q.push(dependent);
                }
            }
        }
        
        return visitedCount == _infos.size();
    }

    void Manager::populateDependents(){
        for (auto& info : _infos){
            if (!info.system){
                // TODO
                continue;
            }
            auto dependencies = info.system->dependencies();

            for (auto dep : dependencies){
                SystemID dependentSysID = getSystemIDFromType(dep.type);

                if (dependentSysID == info.id){
                    throw std::runtime_error("A system is a self dependency : " + std::string(info.system->name()));
                }

                if (dependentSysID != INVALID_SYSTEM_ID){
                    auto& dependentInfo = _infos[dependentSysID];

                    info.dependencies.push_back(dependentSysID);
                    dependentInfo.dependents.push_back(info.id);
                    
                    continue;
                }

                if (!dep.optional){
                    throw std::runtime_error("A system could not resolve dependency : " + std::string(info.system->name()) + " -> " + std::string(dep.type.name()));
                }
            }
        }
    }

    void Manager::setupInitList(){
        _initList.resize(_infos.size());

        std::queue<SystemID> queue;
        std::vector<bool> visited(_infos.size(), 0);

        // Get roots
        for (size_t i = 0; i < _infos.size(); ++i){
            if (_infos[i].dependencies.empty()){
                queue.push(i);
            }
        }

        size_t i=0;
        while (!queue.empty()){
            SystemID currentId = queue.front();
            queue.pop();

            if (visited[currentId]){
                continue;
            }

            visited[currentId] = true;
            _initList[i] = currentId;

            for (auto& dependents : _infos[currentId].dependents){
                queue.push(dependents);
            }

            i++;
        }
    }
}