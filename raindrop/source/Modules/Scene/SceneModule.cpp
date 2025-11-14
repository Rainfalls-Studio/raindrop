#include "Raindrop/Modules/Scene/SceneModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

namespace Raindrop::Scene{
    SceneModule::Name SceneModule::name() const noexcept{
        return "Scene";
    }

    Modules::Result SceneModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        return Modules::Result::Success();
    }
}