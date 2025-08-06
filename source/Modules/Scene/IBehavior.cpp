#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Scene{
    void IBehavior::onCreate(Entity&){}
    void IBehavior::onDestroy(Entity&){}

    void IBehavior::initialize(Engine&,Scene&){}
    void IBehavior::shutdown(){}
}