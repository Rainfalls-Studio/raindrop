#include "Layers/LayerModule.hpp"

extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new Layers::LayerModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}

namespace Layers{
    LayerModule::LayerModule(){}
    LayerModule::~LayerModule(){}

    Raindrop::Result LayerModule::initialize(Raindrop::InitHelper& init){
        _manager = std::make_unique<LayerManager>(init.logger());
        return Raindrop::Result::Success();
    }

    void LayerModule::shutdown(){
        _manager.reset();
    }


    LayerManager* LayerModule::manager(){
        return _manager.get();
    }

    const LayerManager* LayerModule::manager() const{
        return _manager.get();
    }
}