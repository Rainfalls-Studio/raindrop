#include "Raindrop/Modules/Loaders/StaticModuleLoader.hpp"
#include <cassert>

namespace Raindrop::Modules{
    StaticModuleLoader::StaticModuleLoader(CreateFnc create, DestroyFnc destroy) : 
        _create{create},
        _destroy(destroy)
    {
        assert(create && "create must be a valid function");
        assert(destroy && "destroy must be a valid funtion");
    }

    std::shared_ptr<IModule> StaticModuleLoader::create(){
        return std::shared_ptr<IModule>(_create(), _destroy);
    }
}