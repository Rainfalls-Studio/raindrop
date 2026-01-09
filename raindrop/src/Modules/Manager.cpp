#include "Raindrop/Modules/Manager.hpp"
#include "Raindrop/Modules/InitHelper.hpp"
#include "Raindrop/Modules/Instances/DynamicModuleInstance.hpp"

#include <cassert>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>
namespace fs = std::filesystem;

namespace Raindrop{
    ModuleManager::ModuleManager(Engine& engine) : _engine{engine}{
        _logger = spdlog::stdout_color_mt("ModuleModuleManager");
    }
    
    ModuleManager::~ModuleManager(){
        shutdown();
    }

    void ModuleManager::loadModules(const std::filesystem::path& directoryPath){
        _logger->info("Loading modules from \"{}\"", directoryPath.string());

        for (const auto & entry : fs::directory_iterator(directoryPath)){
            if (!entry.is_directory()) continue;
            loadModule(entry.path());
        }
    }

    void ModuleManager::loadModule(const std::filesystem::path& path){
        try{
            registerModule(std::make_unique<DynamicModuleInstance>(path));
        } catch (const std::exception& e){
            _logger->error("Failed to load module at path \"{}\" : {}", path.string(), e.what());
        }
    }

    void ModuleManager::registerModule(std::unique_ptr<IModuleInstance>&& instance){
        assert(instance != nullptr);
        assert(!_initialized && "The module registration must take place before initialization");
        
        std::unique_lock<std::mutex> lock(_mtx);

        const auto& name = instance->name();

        _logger->trace("Registering module \"{}\" (version: {}) ...", name, instance->version());

        // lock.lock();
        // TODO: module override

        if (_nodes.count(instance->name()) != 0){
            _logger->warn("Module override \"{}\"", name);
        }

        auto& node = _nodes[instance->name()];

        node.status = Status::NEW;
        node.instance = std::move(instance);
    }

    void ModuleManager::initializeModule(Node& node){
        Result result = Result::Level::ERROR;
        auto& instance = node.instance;
        Name nodeName = node.name();

        ModuleMap dependencies;

        for (auto & dependency : instance->dependencies()){
            const auto& depName = dependency.get();
            Node& depNode = _nodes[depName];

            dependencies[depName] = depNode.instance->module();
        }

        InitHelper helper(_engine, dependencies);

        _logger->trace("initializing node \"{}\"...", nodeName);

        // node.version = _version;
        result = instance->initialize(helper);
        node.status = catchResultError(nodeName, result);
    }

    Status ModuleManager::catchResultError(const Name& name, const Result& result){

        const std::string result_msg = result.message().empty() ? "" : (": " + result.message());

        // catch result
        switch (result.level()){
            case Result::Level::SUCCESS:{
                _logger->info("Successfully initialized module \"{}\" {} !", name, result_msg);
                return Status::INITIALIZED;
            }

            case Result::Level::ERROR:{
                _logger->warn("Failed to initialize module \"{}\" {}", name, result_msg);
                return Status::FAILED;
            }

            case Result::Level::FATAL:{
                _logger->critical("Failed to initialize critical module \"{}\" {}", name, result_msg);
                throw std::runtime_error(result.message());
            }
        }

        _logger->warn("Unknown result level : {} : {}", static_cast<int>(result.level()), result_msg);
        return Status::FAILED;
    }

    SharedModule ModuleManager::getModule(const std::string& name) noexcept{
        auto& node = _nodes[name];

        if (node.status == Status::INITIALIZED){
            return node.instance->module();
        }

        return {};
    }

    void ModuleManager::initialize(){
        _logger->info("Initializing modules... (n : {})", _nodes.size());

        // Validate deps exist + version constraints.
        for (auto& [name, node] : _nodes) {
            for (auto& d : node.instance->dependencies()) {
                auto it = _nodes.find(d.get());

                bool ok = true;

                if (it == _nodes.end()) {
                    _logger->error("Module \"{}\" depends on missing module \"{}\". Marked as failed", name, d.get());
                    ok = false;

                } else if (Version v = it->second.instance->version(); !d.constraint().satisfies(v)) {
                    _logger->error("Module \"{}\" depends on \"{}\" with incompatible version (version : {} : requirements : {}). Marked as failed", name, it->first, v, d.constraint().toString());
                    ok = false;
                }

                if (!ok){
                    node.status = Status::FAILED;

                    if (auto& instance = node.instance; instance != nullptr && instance->critical()){
                        _logger->critical("Module \"{}\" is missing dependencies and is marked as critical", name);
                        throw std::runtime_error("Critical module is missing dependencies");
                    }
                }
            }
        }

        // Build graph views:
        // inDegree[m] = number of dependencies of m
        std::unordered_map<Name, size_t> inDegree;
        inDegree.reserve(_nodes.size());

        // usersOf[dep] = modules that depend on dep  (adjacency for init)
        std::unordered_map<Name, std::vector<Name>> usersOf;
        usersOf.reserve(_nodes.size());

        for (auto& [name, node] : _nodes) {
            const auto& deps = node.instance->dependencies();

            inDegree[name] = deps.size();

            for (auto& d : deps) {
                usersOf[d.get()].push_back(name);
            }
        }

        // Kahn layering: all nodes with inDegree==0 can init in parallel (no deps).
        std::deque<Name> q;
        for (auto& [name, deg] : inDegree) {
            if (deg == 0) q.push_back(name);
        }

        std::vector<Batch> initList;
        size_t processed = 0;

        while (!q.empty()) {
            // Current layer = everything currently available
            size_t layerCount = q.size();
            Batch batch;
            batch.modules.reserve(layerCount);

            for (size_t i = 0; i < layerCount; ++i) {
                Name n = q.front();
                q.pop_front();

                batch.modules.push_back(n);
            }

            // Optional: deterministic order
            // sort(layer.begin(), layer.end());

            // "Remove" nodes in this layer: decrement inDegree of their users
            for (auto& dep : batch.modules) {
                ++processed;
                auto it = usersOf.find(dep);
                if (it == usersOf.end()) continue;
                for (auto& user : it->second) {
                    size_t& deg = inDegree[user];
                    if (--deg == 0) q.push_back(user);
                }
            }

            initList.push_back(std::move(batch));
        }

        if (processed != _nodes.size()) {
            // There is a cycle. If you want, also extract which nodes remain.
            std::vector<Name> stuck;
            stuck.reserve(_nodes.size());
            for (auto& [name, deg] : inDegree) {
                if (deg > 0) stuck.push_back(name);
            }

            std::string msg = "Dependency cycle detected involving: ";
            for (size_t i = 0; i < stuck.size(); ++i) {
                msg += stuck[i] + (i + 1 < stuck.size() ? ", " : "");
            }

            _logger->error("Failed to initialize modules : {}", msg);
            throw std::runtime_error("Failed to initialize modules");
        }


        _flattenTree = std::move(initList);

        for (const auto& batch : _flattenTree){
            for (const auto& name : batch.modules){
                auto& node = _nodes[name];

                bool ok = true;
                ModuleMap dependencies;

                for (auto& dep : node.instance->dependencies()){

                    auto& depNode = _nodes[dep.get()];
                    if (depNode.status != Status::INITIALIZED){
                        _logger->error("Module \"{}\" depends on failed module \"{}\". Marked as failed", name, dep.get());
                        node.status = Status::FAILED;
                        ok = false;
                        break;
                    }

                    dependencies[dep.get()] = _nodes[dep.get()].instance->module();
                }

                if (!ok || node.status == Status::FAILED){
                    if (node.instance->critical()){
                        _logger->critical("Failed to initialize critcal module \"{}\"", name);
                        throw std::runtime_error("Failed to initialize critical module");
                    }
                    continue;
                }

                InitHelper helper(_engine, dependencies);

                auto result = node.instance->initialize(helper);
                node.status = catchResultError(name, result);

                if (result.level() != Result::Level::SUCCESS && node.instance->critical()){
                    _logger->critical("Failed to initialize critical module \"{}\"", name);
                    throw std::runtime_error("Failed to initialize critical module");
                }
            }
        }

        for (const auto& batch : _flattenTree){
            for (const auto& name : batch.modules){
                auto& node = _nodes[name];

                if (node.status != Status::INITIALIZED) continue;
                node.instance->module()->postInitialize();
            }
        }
    }

    void ModuleManager::shutdown(){
        _logger->info("Shutting down modules...");

        for (auto it = _flattenTree.rbegin(); it != _flattenTree.rend(); it++){
            for (const Name& moduleName : it->modules){
                auto& node = _nodes[moduleName];
                auto& instance = node.instance;

                if (node.status != Status::INITIALIZED){
                    _logger->error("The module \"{}\" tried to shutdown but was not initialized", moduleName);
                    continue;
                }

                if (!instance){
                    _logger->error("The module \"{}\" tried to shutdown but contained no valid instance", moduleName);
                    continue;
                }

                instance->shutdown();
                node.instance.reset();
                node.status = Status::TERMINATED;
            }
        }
    }
}