#include "Raindrop/Graphics/System/Manager.hpp"
#include <queue>
#include <stdexcept>

namespace Raindrop::Graphics::System{
    Manager::Manager(){}
    Manager::~Manager(){}

    void Manager::registerSystem(const std::type_index& type, std::shared_ptr<IRenderSystem>&& system){
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

    std::shared_ptr<IRenderSystem> Manager::getSystem(const std::type_index& type) const{
        return _infos.at(_systems.at(type)).system;
    }
    
    void Manager::foreach(const std::function<void(IRenderSystem&)>& callback){
        for (auto& info : _infos){
            callback(*info.system);
        }
    }

    void Manager::initializeSystems(GraphicsEngine& engine){
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

    Manager::RenderSystemID Manager::getSystemIDFromType(const std::type_index& type){
        try{
            return _systems.at(type);
        } catch (...){
            return INVALID_RENDER_SYSTEM_ID;
        }
    }

    bool Manager::isSystemTreeCyclic(){
        std::vector<size_t> inDegree(_infos.size(), 0);
        std::queue<RenderSystemID> q;

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
            RenderSystemID id = q.front();
            q.pop();
            visitedCount++;

            auto& info = _infos[id];
            
            for (RenderSystemID dependent : info.dependents) {
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
                RenderSystemID dependentSysID = getSystemIDFromType(dep.type);

                if (dependentSysID == info.id){
                    throw std::runtime_error("A system is a self dependency : " + std::string(info.system->name()));
                }

                if (dependentSysID != INVALID_RENDER_SYSTEM_ID){
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

        std::queue<RenderSystemID> queue;
        std::vector<bool> visited(_infos.size(), 0);

        // Get roots
        for (size_t i = 0; i < _infos.size(); ++i){
            if (_infos[i].dependencies.empty()){
                queue.push(i);
            }
        }

        size_t i=0;
        while (!queue.empty()){
            RenderSystemID currentId = queue.front();
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