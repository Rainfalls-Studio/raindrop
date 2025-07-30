#include "Raindrop/Core/Systems/SystemGraph.hpp"
#include "Raindrop/Core/Systems/Errors.hpp"
#include <queue>
#include <list>
#include <spdlog/spdlog.h>

namespace Raindrop::Systems{
    SystemGraph::SystemGraph(){}

    void SystemGraph::push(const std::shared_ptr<ISystem>& system){
        _nodes[system->name()] = {system, {}, {}};
    }

    void SystemGraph::pop(const std::shared_ptr<ISystem>& system){
        _nodes.erase(system->name());
    }

    void SystemGraph::popAndDependents(const std::shared_ptr<ISystem>& system){

        std::queue<std::string> toPop;
        std::queue<Map::iterator> toScan;

        toScan.push(_nodes.find(system->name()));
        
        while (!toScan.empty()){
            Map::iterator current = toScan.front();
            toScan.pop();

            if (current == _nodes.end()) continue;

            toPop.push(current->first);

            auto& dependents = current->second.dependents;

            for (auto& dependent : dependents){
                toScan.push(_nodes.find(dependent));
            }
        }

        while (!toPop.empty()){
            auto current = toPop.front();
            toPop.pop();

            _nodes.erase(current);
        }
    }

    void SystemGraph::clear(){
        _nodes.clear();
    }

    bool isGraphCyclic(SystemGraph::Map& nodes){
        std::unordered_map<std::string, size_t> inDegree(nodes.size());
        std::queue<std::string> q;

        size_t visitedCount = 0;
        
        // Calculate in-degree for each node
        for (const auto& [it, node] : nodes) {
            inDegree[node.name()] = node.dependencies.size();
        }
        
        // Enqueue nodes with in-degree 0
        for (auto [node, degree] : inDegree){
            if (degree == 0){
                q.push(node);
            }
        }
        
        while (!q.empty()) {
            std::string system = q.front();
            q.pop();
            visitedCount++;

            auto& node = nodes.at(system);
            
            for (const std::string& dependent : node.dependents) {
                inDegree[dependent]--;
                if (inDegree[dependent] == 0) {
                    q.push(std::string(dependent));
                }
            }
        }
        
        return visitedCount != nodes.size();
    }

    std::expected<void, Error> populateGraph(SystemGraph::Map& nodes){
        // For each node
        for (auto [it, rich] : nodes){
            auto& system = rich.system;
            
            // list dependencies
            auto dependencies = system->dependencies();
            
            // For each dependency
            for (auto dep : dependencies){
                auto dependencyName = dep.getDependency();
                
                // Check if dependency in graph
                if (nodes.count(dependencyName) == 0){
                    if (!dep.isOptional()){
                        return std::unexpected(Error(make_error_code(ErrorCodes::MISSING_DEPENDENCY), "system \"{}\" is missing dependency \"{}\"", rich.name(), dependencyName));
                    }
                }

                rich.dependencies.push_back(dependencyName);
            }
        }

        return {};
    }

    std::list<std::string> getRoots(const SystemGraph::Map& graph){
        std::list<std::string> roots;

        for (auto [it, rich] : graph){
            if (rich.dependencies.empty()){
                roots.push_back(rich.name());
            }
        }

        return roots;
    }


    std::list<std::string> getLeafs(const SystemGraph::Map& graph){
        std::list<std::string> leafs;

        for (auto [it, rich] : graph){
            if (rich.dependents.empty()){
                leafs.push_back(rich.name());
            }
        }

        return leafs;
    }

    std::expected<void, Error> SystemGraph::initialize(Engine& engine){
        auto result = populateGraph(_nodes);

        if (!result){
            return std::unexpected(result.error());
        }

        if (isGraphCyclic(_nodes)){
            return std::unexpected(Error(make_error_code(ErrorCodes::CYCLIC), "Graph is cyclic"));
        }

        std::queue<std::string> q;
        
        {
            auto roots = getRoots(_nodes);
            for (auto root : roots){
                q.push(root);
            }
        }

        while (!q.empty()){
            auto currentName = std::move(q.front());
            auto current = _nodes.at(currentName);
            q.pop();

            if (_nodes.count(currentName) == 0){
                continue;
            }
            
            auto initResult = current.system->initialize(engine);

            if (!initResult){
                auto& error = result.error();
                spdlog::error("Failed to initialize system {} with error {} || {}", currentName, error.message(), error.reason());
                popAndDependents(current.system);
                continue;
            }

            for (auto dependent : current.dependents){
                q.push(dependent);
            } 
        }

        for (auto [it, node] : _nodes){
            node.system->postInitialize();
        }

        return {};
    }

    void SystemGraph::shutdown(){
        for (auto [it, node] : _nodes){
            node.system->preShutdown();
        }

        std::queue<std::string> q;

        {
            auto leafs = getLeafs(_nodes);
            for (auto leaf : leafs){
                q.push(leaf);
            }
        }

        while (!q.empty()){
            auto currentName = std::move(q.front());
            auto current = _nodes.at(currentName);
            q.pop();

            if (_nodes.count(currentName) == 0){
                continue;
            }

            current.system->shutdown();

            for (auto dependency : current.dependencies){
                q.push(dependency);
            }

            pop(current.system);
        }
    }

    const SystemGraph::Map& SystemGraph::getNodes(){
        return _nodes;
    }

}