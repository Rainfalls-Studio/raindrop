#include "Scene/SceneModule.hpp"
#include <Raindrop/Modules/InitHelper.hpp>

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Scene::SceneModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}


namespace Raindrop::Scene{
    Modules::Name SceneModule::name() const noexcept{
        return RAINDROP_CURRENT_MODULE_NAME;
    }

    Modules::Result SceneModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        return Modules::Result::Success();
    }
}