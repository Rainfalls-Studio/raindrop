#include "Scene/SceneModule.hpp"
#include <Raindrop/Modules/InitHelper.hpp>

namespace Raindrop::Scene{
    SceneModule::Name SceneModule::name() const noexcept{
        return RAINDROP_CURRENT_MODULE_NAME;
    }

    Modules::Result SceneModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        return Modules::Result::Success();
    }
}