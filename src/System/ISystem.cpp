#include "Raindrop/System/ISystem.hpp"

namespace Raindrop::System{
    void ISystem::initialize(Engine&){}
    void ISystem::postInitialize(){}
    void ISystem::preShutdown(){}
    void ISystem::shutdown(){}

    void ISystem::preEvent(){}
    void ISystem::event(){}
    void ISystem::postEvent(){}

    void ISystem::preUpdate(){}
    void ISystem::update(){}
    void ISystem::postUpdate(){}
    
    void ISystem::preRender(){}
    void ISystem::render(){}
    void ISystem::postRender(){}
    std::vector<ISystem::Dependency> ISystem::dependencies() const{return {};}
    void ISystem::dependencyShutdown(const std::type_index&, const ISystem&){}

}