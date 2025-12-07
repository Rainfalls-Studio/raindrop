#include "Layers/LayerModule.hpp"

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Layers::LayerModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}

namespace Raindrop::Layers{
    LayerModule::LayerModule(){}
    LayerModule::~LayerModule(){}

    Modules::Result LayerModule::initialize(Modules::InitHelper&){
        _manager = std::make_unique<LayerManager>();
        return Modules::Result::Success();
    }

    void LayerModule::shutdown(){
        _manager.reset();
    }
 
    Modules::Name LayerModule::name() const noexcept{
        return RAINDROP_CURRENT_MODULE_NAME;
    }

    Modules::DependencyList LayerModule::dependencies() const noexcept{
        return {};
    }

    LayerManager* LayerModule::manager(){
        return _manager.get();
    }

    const LayerManager* LayerModule::manager() const{
        return _manager.get();
    }
}