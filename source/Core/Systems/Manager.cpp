#include "Raindrop/Core/Systems/Manager.hpp"
#include "Raindrop/Core/Systems/Errors.hpp"
#include "Raindrop/Core/Error.hpp"

#include <cassert>
#include <spdlog/spdlog.h>

namespace Raindrop::Systems{
    Manager::Manager(Engine& engine) : _engine{engine}{}

    std::expected<void, Error> Manager::registerSystem(const std::shared_ptr<ISystem>& system){
        if (_initialized){
            return std::unexpected(Error(make_error_code(ErrorCodes::ALREADY_INITIALIZED), "Cannot register system after initialization"));
        }

        _graph.push(system);
        return {};
    }

    std::shared_ptr<ISystem> Manager::getSystem(const std::string& name) noexcept{
        auto& nodes = _graph.getNodes();

        auto it = nodes.find(name);
        if (it != nodes.end()){
            return it->second.system;
        }

        return {};
    }

    std::expected<void, Error> Manager::initialize(){
        auto result = _graph.initialize(_engine);

        if (!result){
            return std::unexpected(result.error());
        }

        _initialized = true;
        return {};
    }

    
    void Manager::shutdown(){
        _graph.shutdown();
    }
}