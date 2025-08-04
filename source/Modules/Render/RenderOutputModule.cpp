#include "Raindrop/Modules/Render/RenderOutputModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    class RenderOutputErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Render Output";
            }

            std::string message(int ev) const override{
                using ErrorCode = RenderOutputModule::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::MISSING_SURFACE_SUPPORT: return "Missing surface support";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& RenderOutputModule::error_category(){
        static RenderOutputErrorCategory category;
        return category;
    }

    RenderOutputModule::RenderOutputModule(){}
    RenderOutputModule::~RenderOutputModule(){}

    Modules::Result RenderOutputModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        auto scheduler = helper.getDependencyAs<RenderSchedulerModule>("RenderScheduler");

        scheduler->setPreparRenderCallback([this]{prepareRender();});
        scheduler->setPreRenderCallback([this] -> RenderSchedulerModule::PreRenderResult {return preRender();});
        scheduler->setPostRender([this](const RenderSchedulerModule::RenderResult& renderResult){postRender(renderResult);});

        return Modules::Result::Success();
    }

    void RenderOutputModule::shutdown(){
        shutdownAllOutputs();
        _outputs.clear();
    }

    RenderOutputModule::Name RenderOutputModule::name() const noexcept{
        return "RenderOutput";
    }

    Modules::DependencyList RenderOutputModule::dependencies() const noexcept{
        return {
            Modules::Dependency("RenderCore"),
            Modules::Dependency("RenderScheduler"),
        };
    }

    Modules::Result RenderOutputModule::dependencyReload(const Name& name){
        if (name == "RenderCore"){
            _core = _engine->getModuleManager().getModuleAs<RenderCoreModule>("RenderCore");
            rebuildAllOutputs();
        }
        return Modules::Result::Success();
    }

    Modules::Result RenderOutputModule::dependencyShutdown(const Name&){
        return Modules::Result::Error();
    }

    void RenderOutputModule::initializeAllOutputs(){
        for (auto [it, info] : _outputs){
            spdlog::info("Initializing render output \"{}\"", it);
            info->output->initialize(*_core);
        }
    }

    void RenderOutputModule::shutdownAllOutputs(){
        for (auto [it, info] : _outputs){
            spdlog::info("Shuting down render output \"{}\"", it);
            info->output->shutdown();
        }
    }

    void RenderOutputModule::rebuildAllOutputs(){
        for (auto [it, info] : _outputs){
            spdlog::info("Rebuilding render output \"{}\"", it);
            info->output->shutdown();
            info->output->initialize(*_core);
        }
    }

    SharedRenderOutput RenderOutputModule::getOutput(const IRenderOutput::Name& name){
        auto it = _outputs.find(name);
        if (it == _outputs.end()) return nullptr;

        return it->second->output;
    }

    void RenderOutputModule::registerOutput(const IRenderOutput::Name& name, SharedRenderOutput output){
        spdlog::info("Registering and initializing render output \"{}\"", name);
        
        if (_outputs.count(name) > 0){
            spdlog::info("Overwriting existing output \"{}\"", name);
        }
        _outputs[name] = std::make_shared<RenderOutputInfo>(output, name);
        output->initialize(*_core);
        findMainOutput();
    }

    void RenderOutputModule::unregisterOutput(const IRenderOutput::Name& name){
        _outputs.erase(name);
        findMainOutput();
    }

    void RenderOutputModule::setMainOutput(const IRenderOutput::Name& name){
        _mainOutputName = name;
        findMainOutput();
    }

    void RenderOutputModule::findMainOutput(){
        if (_outputs.empty()){
            spdlog::error("There are no registred render output !");
            _mainOutput = nullptr;
            return;
        }

        auto it = _outputs.find(_mainOutputName);
        if (it == _outputs.end()){
            _mainOutput = findFirstValid();

            if (_mainOutput)
                spdlog::error("No render outputs match's the main output's name (\"{}\"). Falling back to \"{}\"", _mainOutputName, _mainOutput->name);

            return;
        }

        _mainOutput = it->second;
        if (!_mainOutput && !_mainOutput->valid()){
            _mainOutput = findFirstValid();

            if (_mainOutput)
                spdlog::error("Found the main output, but it is not valid. Falling back to \"{}\"", _mainOutput->name);
        }
    }

    std::shared_ptr<RenderOutputModule::RenderOutputInfo> RenderOutputModule::findFirstValid(){
        for (auto [it, info] : _outputs){
            if (info && info->valid()){
                return info;
            }
        }
        spdlog::error("There are no valid outputs registred !");
        return nullptr;
    }


    RenderOutputModule::RenderOutputResult RenderOutputModule::acquireRenderOutput(std::shared_ptr<RenderOutputInfo> info, uint64_t timeout){
        auto result = info->output->acquire(timeout);

        if (!result){
            const auto& error = result.error();

            spdlog::error("Failed to acquire render output \"{}\" : {} : {}", info->name, error.message(), error.message());
            info->available = false;
            return RenderOutputResult::ERROR;
        }

        info->available = *result;
        return info->available ? RenderOutputResult::SUCCESS : RenderOutputResult::SKIP;
    }

    void RenderOutputModule::prepareRender(){
        if (!_mainOutput) return;

        RenderOutputResult result = acquireRenderOutput(_mainOutput);
        
        if (result == RenderOutputResult::SKIP){
            return;

        } else if (result == RenderOutputResult::ERROR){
            spdlog::error("Error occured while acquiering main render output (\"{}\") ! discarding output", _mainOutputName);
            _mainOutput->enabled = false;
            findMainOutput();
            return;
        }
    }
    
    RenderOutputModule::RenderOutputResult RenderOutputModule::presentRenderOutput(std::shared_ptr<RenderOutputInfo> info, const RenderSchedulerModule::RenderResult& results){
        if (info->valid()){
            auto result = info->output->present(results.signal);

            if (!result){
                const auto& error = result.error();
                spdlog::error("Failed to present render output \"{}\" : {}, {}", info->name, error.message(), error.reason());
                return RenderOutputResult::ERROR;
            }
            return RenderOutputResult::SUCCESS;
        }
        return RenderOutputResult::SKIP;
    }

    void RenderOutputModule::postRender(const RenderSchedulerModule::RenderResult& results){
        if (!_mainOutput) return;
        
        auto result = presentRenderOutput(_mainOutput, results);
        
        if (result == RenderOutputResult::ERROR){
            spdlog::error("Error occured while presenting main render output (\"{}\") ! discarding output", _mainOutputName);
            _outputs.erase(_mainOutputName);
        }

        _mainOutput->available = false;
    }

    RenderSchedulerModule::PreRenderResult RenderOutputModule::preRender(){
        if (!_mainOutput) return {};

        auto result = _mainOutput->output->preRender();

        if (!result){
            const auto& error = result.error();
            spdlog::error("Failed to get pre render result for render output \"{}\" : {}, {}", _mainOutputName, error.message(), error.reason());
            return {};
        }

        auto out = *result;
        RenderSchedulerModule::PreRenderResult preSubmitResult{
            .wait = out.wait,
            .waitStageFlags = out.waitStageFlags
        };
            
        return preSubmitResult;
    }
}