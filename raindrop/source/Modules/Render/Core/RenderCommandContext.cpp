#include "Raindrop/Modules/Render/Core/RenderCommandContext.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Render{

    class RenderCommandContextErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Render Command Context";
            }

            std::string message(int ev) const override{
                using ErrorCode = RenderCommandContext::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::UNKNOWN_QUEUE_TYPE: return "Unknown queue type";
                    case ErrorCode::FAILED_POOL_CREATION: return "Failed pool creation";
                    case ErrorCode::FAILED_COMMAND_BUFFER_ALLOCATION: return "Failed command buffer allocation";
                    case ErrorCode::FAILED_FENCE_CREATION: return "Failed to create fences";
                    case ErrorCode::FAILED_SEMAPHORE_CREATION: return "Failed to create semaphores";
                    case ErrorCode::ALREADY_RECORDING: return "A buffer is already recoring";
                    case ErrorCode::NOT_RECORDING: return "No buffer is recording";
                    case ErrorCode::FAILED_BEGIN_RECORD: return "Failed to begin the recording of a command buffer";
                    case ErrorCode::FAILED_END_RECORD: return "Failed to end the recording of a command buffer";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& RenderCommandContext::error_category(){
        static RenderCommandContextErrorCategory category;
        return category;
    }

    RenderCommandContext::RenderCommandContext(std::shared_ptr<RenderCoreModule> core, Queue::Type type,  uint32_t bufferCount, const std::string& name) : 
        _name{name.empty() ? "unamed render command context" : name},
        _type{type},
        _core{core},
        _wantedBufferCount{bufferCount}
    {
        auto result = createPool()
            .and_then([&](){return allocateBuffers(bufferCount);})
            .and_then([&](){return createFences();})
            .and_then([&](){return createSemaphores();});

        if (!result){
            auto error = result.error();
            spdlog::error("{}, {}", error.message(), error.reason());
            throw std::runtime_error("Failed to create render command context");
        }
    }

    RenderCommandContext::~RenderCommandContext(){
        _skipFrame = true;
        waitForFences();

        destroyPool();
        destroyFences();
        destroySemaphores();
    }

    void RenderCommandContext::destroyPool(){
        auto device = _core->deviceManager().device();

        if (_pool){
            device.destroyCommandPool(_pool);
        }
    }

    std::expected<void, Error> RenderCommandContext::createPool(){

        // getting family index
        uint32_t familyIndex = 0;

        switch (_type){
            case Queue::Type::GRAPHICS: familyIndex = _core->graphicsQueue().familyIndex(); break;
            case Queue::Type::COMPUTE: familyIndex = _core->computeQueue().familyIndex(); break;
            case Queue::Type::TRANSFER: familyIndex = _core->transferQueue().familyIndex(); break;
            default:
                return std::unexpected(Error(UnknownQueueTypeError(), "unknown queue type (type={})", static_cast<int>(_type)));
        }

        // pool creation
        vk::CommandPoolCreateInfo info{
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient,
            familyIndex
        };
        
        auto device = _core->deviceManager().device();

        // create pool
        {
            auto result = device.createCommandPool(info);
            if (result.result == vk::Result::eSuccess){
                _pool = result.value;

            } else {
                spdlog::error("Failed to create render command context command pool");
                return std::unexpected<Error>(Error(FailedPoolCreationError(), vk::to_string(result.result)));
            }
        }

        #if 0
            // set debug name
            vk::DebugUtilsObjectNameInfoEXT debugInfo{
                vk::ObjectType::eCommandPool,
                reinterpret_cast<uint64_t>(static_cast<VkCommandPool>(_pool)),
                (_name + "- command pool").c_str()
            };

            if (auto result = device.setDebugUtilsObjectNameEXT(debugInfo); result != vk::Result::eSuccess){
                spdlog::warn("Failed to set debug name for render command context \"{}\" command pool : ", _name, vk::to_string(result));
            }
        #endif

        return {};
    }


    std::expected<vk::CommandBuffer, Error> RenderCommandContext::begin(){
        if (_recording){
            return std::unexpected(Error(AlreadyRecordingError(), "Buffer (id={}) is already in recording state", _currentIndex));
        }

        #if 0
            if (_buffers.size() != _wantedBufferCount){

                // want more buffers -> difference tells the number of buffers to allocate
                // want less -> -difference tells the number of buffers to free
                int32_t difference = static_cast<int32_t>(_wantedBufferCount) - static_cast<int32_t>(_buffers.size());
                uint32_t before = static_cast<uint32_t>(_buffers.size());

                if (difference > 0){
                    _buffers.resize(_wantedBufferCount);
                    auto result = allocateBuffers(static_cast<uint32_t>(difference), before);
                    if (!result){
                        return std::unexpected(result.error());
                    }
                } else {
                    auto result = freeCommandBuffers(static_cast<uint32_t>(-difference), _currentIndex+1);
                    if (!result){
                        return std::unexpected(result.error());
                    }
                }
            }
        #endif

        auto& buffer = _buffers[_currentIndex];
        // buffer.inUse = false;

        auto cmdBuffer = buffer.cmdBuffer;

        assert(cmdBuffer);

        vk::CommandBufferBeginInfo info{};

        if (auto result = cmdBuffer.begin(info); result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedBeginRecordError(), "Failed to begin recording of command buffer (id={}) : {}", _currentIndex, vk::to_string(result)));
        }

        _recording=true;

        return cmdBuffer;
    }

    std::expected<void, Error> RenderCommandContext::end(){
        if (!_recording){
            return std::unexpected(Error(NotRecordingError(), "No buffer is currently in record state"));
        }

        auto buffer = _buffers[_currentIndex].cmdBuffer;
        _recording=false;

        if (auto result = buffer.end(); result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedEndRecordError(), "Failed to end recording of command buffer (id={}) : ", _currentIndex, vk::to_string(result)));
        }

        return {};
    }

    void RenderCommandContext::nextFrame(){
        _currentIndex = (_currentIndex + 1) % _bufferCount;
    }

    std::expected<void, Error> RenderCommandContext::allocateBuffers(uint32_t n){
        vk::CommandBufferAllocateInfo info(
            _pool,
            vk::CommandBufferLevel::ePrimary,
            n
        );

        // std::vector<vk::CommandBuffer>
        auto device = _core->deviceManager().device();
        auto result = device.allocateCommandBuffers(info);

        if (result.result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedCommandBufferAllocationError(), "Failed to allocate {} command bufffer(s) : {}", n, vk::to_string(result.result)));
        }

        auto& buffers = result.value;

        // setup debug names
        #if 0
            for (size_t i=0; i<n; i++){
                auto& buffer = buffers[i];
                // set debug name
                vk::DebugUtilsObjectNameInfoEXT debugInfo{
                    vk::ObjectType::eCommandBuffer,
                    reinterpret_cast<uint64_t>(static_cast<VkCommandBuffer>(buffer)),
                    (_name + "- command buffer " + std::to_string(i)).c_str()
                };

                if (auto result2 = device.setDebugUtilsObjectNameEXT(debugInfo); result2 != vk::Result::eSuccess){
                    spdlog::warn("Failed to set debug name for render command context \"{}\" command buffer : ", _name, vk::to_string(result2));
                }
            }
        #endif

        _bufferCount = n;

        _buffers.resize(_bufferCount);
        for (size_t i=0; i<_bufferCount; i++){
            _buffers[i].cmdBuffer = buffers[i];
        }


        return {};
    }

    std::expected<void, Error> RenderCommandContext::createFences(){
        auto device = _core->deviceManager().device();

        vk::FenceCreateInfo info{
            vk::FenceCreateFlagBits::eSignaled
        };

        for (uint32_t i=0; i<_bufferCount; i++){
            auto& fence = _buffers[i].fence;
            auto result = device.createFence(info);

            if (result.result != vk::Result::eSuccess){
                return std::unexpected(Error(FailedFenceCreationError(), "Failed to create fence (id={}) : {}", i, vk::to_string(result.result)));
            }

            fence = result.value;
        }

        return {};
    }

    std::expected<void, Error> RenderCommandContext::createSemaphores(){
        auto device = _core->deviceManager().device();

        vk::SemaphoreCreateInfo info{};

        for (uint32_t i=0; i<_bufferCount; i++){
            auto& semaphore = _buffers[i].semaphore;
            auto result = device.createSemaphore(info);

            if (result.result != vk::Result::eSuccess){
                return std::unexpected(Error(FailedSemaphoreCreationError(), "Failed to create semaphore (id={}) : {}", i, vk::to_string(result.result)));
            }

            semaphore = result.value;
        }

        return {};
    }


    void RenderCommandContext::freeCommandBuffers(){
        auto device = _core->deviceManager().device();

        std::vector<vk::CommandBuffer> cmdBuffers(_bufferCount);
        for (size_t i=0; i<_bufferCount; i++){
            cmdBuffers[i] = _buffers[i].cmdBuffer;
        }

        device.freeCommandBuffers(
            _pool,
            cmdBuffers
        );
    }

    void RenderCommandContext::destroyFences(){
        auto device = _core->deviceManager().device();

        for (auto& buffer : _buffers){
            device.destroyFence(buffer.fence);    
        }
    }

    void RenderCommandContext::destroySemaphores(){
        auto device = _core->deviceManager().device();

        for (auto& buffer : _buffers){
            device.destroySemaphore(buffer.semaphore);
        }
    }


    RenderCommandContext::Buffer& RenderCommandContext::currentBuffer(){
        return _buffers[_currentIndex];
    }

    void RenderCommandContext::waitFor(vk::Semaphore semaphore, vk::PipelineStageFlags stage){
        _waitSemaphores.push_back({
            semaphore,
            stage
        });
    }

    void RenderCommandContext::skipFrame(){
        _skipFrame = true;
    }

    bool RenderCommandContext::SkipingFrame() const{
        return _skipFrame;
    }

    std::vector<RenderCommandContext::SemaphoreWait>& RenderCommandContext::semaphoreWaits(){
        return _waitSemaphores;
    }

    void RenderCommandContext::waitForFences(){
        auto device = _core->deviceManager().device();

        std::vector<vk::Fence> fences;

        for (auto& buffer : _buffers){
            if (buffer.inUse){
                fences.push_back(buffer.fence);
                spdlog::info("waiting fence");
            }
        }

        if (auto result = device.waitForFences(fences, vk::True, UINT64_MAX); result != vk::Result::eSuccess){
            spdlog::warn("Failed to wait for fences : {}", vk::to_string(result));
        }
    }

    std::shared_ptr<RenderCoreModule> RenderCommandContext::core(){
        return _core;
    }



    // ===================================================================




    RenderCommandContext::BeginStage::BeginStage(const std::shared_ptr<RenderCommandContext>& ctx) : _ctx{ctx}{}
    RenderCommandContext::BeginStage::~BeginStage(){}

    const char* RenderCommandContext::BeginStage::name() const{
        return "Render command context - begin stage";
    }

    void RenderCommandContext::BeginStage::initialize(Scheduler::StageInitHelper&){}

    Scheduler::StageResult RenderCommandContext::BeginStage::execute(){
        auto ctx = _ctx.lock();

        if (!ctx){
            return Scheduler::StageResult::Skip("Context is invalid");
        }

        ctx->nextFrame();

        if (auto result = ctx->begin(); !result){
            auto& error = result.error();
            return Scheduler::StageResult::Skip("Failed to begin command buffer : " + error.message());
        }
        
        return Scheduler::StageResult::Continue();
    }



    // ===================================================================



    RenderCommandContext::EndStage::EndStage(const std::shared_ptr<RenderCommandContext>& ctx) : _ctx{ctx}{}
    RenderCommandContext::EndStage::~EndStage(){}

    const char* RenderCommandContext::EndStage::name() const{
        return "Render command context - end stage";
    }

    void RenderCommandContext::EndStage::initialize(Scheduler::StageInitHelper&){}

    Scheduler::StageResult RenderCommandContext::EndStage::execute(){
        auto ctx = _ctx.lock();

        if (!ctx){
            return Scheduler::StageResult::Skip("Context is invalid");
        }

        if (auto result = ctx->end(); !result){
            auto& error = result.error();
            return Scheduler::StageResult::Skip("Failed to end command buffer : " + error.message());
        }
        
        return Scheduler::StageResult::Continue();
    }



    // ===================================================================


    RenderCommandContext::SubmitStage::SubmitStage(const std::shared_ptr<RenderCommandContext>& ctx, Queue queue) :
        _ctx{ctx},
        _queue{queue}  
    {}

    RenderCommandContext::SubmitStage::~SubmitStage(){}

    const char* RenderCommandContext::SubmitStage::name() const{
        return "Render command context - submit stage";
    }

    void RenderCommandContext::SubmitStage::initialize(Scheduler::StageInitHelper&){}

    Scheduler::StageResult RenderCommandContext::SubmitStage::execute(){
        auto ctx = _ctx.lock();

        if (!ctx){
            return Scheduler::StageResult::Skip("Context is invalid");
        }

        if (ctx->SkipingFrame()){
            spdlog::info("skipping frame !");
            ctx->_skipFrame = false;
            return Scheduler::StageResult::Continue();
        }

        auto device = ctx->core()->deviceManager().device();

        auto& buffer = ctx->currentBuffer();


        vk::CommandBuffer commandBuffer = buffer.cmdBuffer;
        vk::Semaphore signalSemaphore = buffer.semaphore;
        vk::Fence fence = buffer.fence;

        std::vector<SemaphoreWait>& waits = ctx->semaphoreWaits();

        std::vector<vk::Semaphore> waitSemaphores(waits.size());
        std::vector<vk::PipelineStageFlags> waitStages(waits.size());

        for (size_t i=0; i<waits.size(); i++){
            const auto& wait = waits[i];
            waitSemaphores[i] = wait.semaphore;
            waitStages[i] = wait.stage;
        }

        waits.clear();

        if (auto result = device.resetFences(fence); result != vk::Result::eSuccess){
            spdlog::warn("Failed to reset fence : {}", vk::to_string(result));
        }

        vk::SubmitInfo info{
        static_cast<uint32_t>(waitSemaphores.size()),
            waitSemaphores.data(),
            waitStages.data(),
            1, &commandBuffer,
            1, &signalSemaphore
        };

        if (auto result = _queue->submit(info, fence); result != vk::Result::eSuccess){
            Scheduler::StageResult::Skip("Failed to submit render command context - " + vk::to_string(result));
        }

        buffer.inUse = true;
        
        return Scheduler::StageResult::Continue();
    }
}