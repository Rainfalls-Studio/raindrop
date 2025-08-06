#include "Raindrop/Modules/Scene/SceneModule.hpp"
#include "Raindrop/Modules/Scene/SceneTrait.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"
#include "Raindrop/Core/Layers/Layer.hpp"

namespace Raindrop::Scene{
    SceneModule::Name SceneModule::name() const noexcept{
        return "Scene";
    }

    Modules::Result SceneModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        return Modules::Result::Success();
    }

    SceneTrait& SceneModule::emplaceTrait(Layers::Layer layer){
        auto& trait = layer.emplaceTrait<SceneTrait>();
        trait.scene.initialize(*_engine);
        return trait;
    }

}