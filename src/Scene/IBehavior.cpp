#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Scene{
    void IBehavior::initialize(){}
    void IBehavior::shutdown(){}

    void IBehavior::preEvent(){}
    void IBehavior::event(){}
    void IBehavior::postEvent(){}

    void IBehavior::preUpdate(){}
    void IBehavior::update(){}
    void IBehavior::postUpdate(){}

    void IBehavior::preRender(){}
    void IBehavior::render(){}
    void IBehavior::postRender(){}
}