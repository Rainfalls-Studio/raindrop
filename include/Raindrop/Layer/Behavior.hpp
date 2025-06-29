#pragma once

#include "Layer.hpp"
#include "Raindrop/Core/Utils/RegistryBehavior.hpp"
#include "Raindrop/Layer/Manager.hpp"

namespace Raindrop::Layer{
    using Behavior = Core::Utils::RegistryBehavior<LayerID, std::shared_ptr<Layer>>;
}