#include "Raindrop/Graphics/System/IRenderSystem.hpp"

namespace Raindrop::Graphics::System{
    void IRenderSystem::initialize(GraphicsEngine&){}
    void IRenderSystem::postInitialize(){}
    void IRenderSystem::preShutdown(){}
    void IRenderSystem::shutdown(){}
    
    void IRenderSystem::preRender(){}
    void IRenderSystem::render(){}
    void IRenderSystem::postRender(){}
    std::vector<IRenderSystem::Dependency> IRenderSystem::dependencies() const{return {};}
    void IRenderSystem::dependencyShutdown(const std::type_index&, const IRenderSystem&){}

}