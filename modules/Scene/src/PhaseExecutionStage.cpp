#include "Scene/PhaseExecutionStage.hpp"

namespace Raindrop::Scene{
    PhaseExecutionStage::PhaseExecutionStage(std::shared_ptr<Scene> scene, PhaseID phase) : 
        _scene{scene},
        _phase{phase}
    {}

    const char* PhaseExecutionStage::name() const{
        return "Scene - Phase execution stage";
    }

    Scheduler::StageResult PhaseExecutionStage::execute(){
        auto scene = _scene.lock();

        if (!scene){
            return Scheduler::StageResult::Skip("No scene");
        }

        scene->executePhase(_phase);
        return Scheduler::StageResult::Continue();
    }
}