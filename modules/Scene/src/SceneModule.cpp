#include "Scene/SceneModule.hpp"
#include <Raindrop/Modules/InitHelper.hpp>

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Scene::SceneModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}


namespace Raindrop::Scene{
    Modules::Result SceneModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        return Modules::Result::Success();
    }
}