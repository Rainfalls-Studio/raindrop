#include "Raindrop/Modules/Instances/StaticModuleInstance.hpp"
#include <cassert>

namespace Raindrop::Modules{
    StaticModuleInstance::StaticModuleInstance(
        CreateFnc create,
        DestroyFnc destroy,
        Name name,
        Version version,
        DependencyList dependencies,
        bool critical
    ) : 
        _create{create},
        _destroy(destroy),
        _name(name),
        _version(version),
        _dependencies(std::move(dependencies)),
        _critical(critical)
    {
        assert(create && "create must be a valid function");
        assert(destroy && "destroy must be a valid funtion");
        assert(!name.empty() && "The module name most not be empty");
    }

    const DependencyList& StaticModuleInstance::dependencies() const{
        return _dependencies;
    }

    const Name& StaticModuleInstance::name() const{
        return _name;
    }

    const Version& StaticModuleInstance::version() const{
        return _version;
    }

    bool StaticModuleInstance::critical() const{
        return _critical;
    }

    Result StaticModuleInstance::initialize(InitHelper& helper){
        auto module = _create();
        _module = std::shared_ptr<IModule>(module, _destroy);

        return _module->initialize(helper);
    }

    void StaticModuleInstance::shutdown(){
        _module->shutdown();
        _module.reset();
    }

    std::shared_ptr<IModule> StaticModuleInstance::module() const{
        return _module;
    }
}