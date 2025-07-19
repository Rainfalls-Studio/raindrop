#include "Raindrop/Graphics/Output/OutputSystem.hpp"

namespace Raindrop::Graphics::Output{
    void OutputSystem::initialize(GraphicsEngine& engine){
        _engine = &engine;
    }

    void OutputSystem::shutdown(){
        _output.reset();
    }

    const char* OutputSystem::name() const{
        return "Output system";
    }

    void OutputSystem::setOutput(const std::shared_ptr<IOutputTarget>& output){
        _output = output;
    }

    std::shared_ptr<IOutputTarget> OutputSystem::getOutput(){
        return _output.lock();
    }
}