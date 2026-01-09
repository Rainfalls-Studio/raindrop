#include "Scene/SceneModule.hpp"
#include <Raindrop/Modules/InitHelper.hpp>

extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new Raindrop::Scene::SceneModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}


namespace Raindrop::Scene{
    Result SceneModule::initialize(InitHelper& helper){
        _engine = &helper.engine();

        return Result::Success();
    }
}