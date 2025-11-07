// #include "Planet.hpp"
// #include <glm/gtc/matrix_transform.hpp>

// struct PushConstant{
//     glm::mat4 transform;
//     glm::mat4 viewTransform;
// };

// struct Vertex{
//     glm::vec3 position;
//     glm::vec2 UV;
// };

// struct Instance{
//     glm::vec2 position;
//     uint8_t face;
//     float scale;
// };

// Planet::RenderStage::RenderStage(std::weak_ptr<Planet> planet, std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx) : 
//     _planet{planet},
//     _output{output},
//     _cmdCtx(cmdCtx)
// {}

// const char* Planet::RenderStage::name() const{
//     return "Planet - Render Stage";
// }

// void Planet::RenderStage::initialize(Raindrop::Scheduler::StageInitHelper&){}
// void Planet::RenderStage::shutdown(){}

// Raindrop::Scheduler::StageResult Planet::RenderStage::execute(){
//     using namespace Raindrop::Scheduler;
//     auto output = _output.lock();

//     if (!output){
//         return StageResult::Skip("No output");
//     }

//     auto cmdCtx = _cmdCtx.lock();

//     if (!cmdCtx){
//         return StageResult::Skip("No render command context");
//     }

//     auto planet = _planet.lock();

//     if (!planet){
//         return StageResult::Skip("No planet to render");
//     }

//     // planet->_pipeline

//     vk::CommandBuffer cmd = cmdCtx->currentBuffer().cmdBuffer;

//     auto extent = output->extent();
//     vk::Viewport viewport{
//         0.f, 0.f,
//         static_cast<float>(extent.width),
//         static_cast<float>(extent.height),
//         0.f, 1.f
//     };

//     cmd.setViewport(0, viewport);

//     vk::Rect2D scissor{
//         {},
//         extent
//     };

//     cmd.setScissor(0, scissor);

//     cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, planet->_pipeline);

//     double a = Raindrop::Time::now().as<Raindrop::Time::milliseconds>().count() / 1000.f;

//     PushConstant pc{
//         glm::rotate(glm::mat4(1.f), float(a), glm::vec3(0.f, 0.f, 1.f)),
//         glm::mat4(1.f)
//     };

//     cmd.pushConstants<PushConstant>(planet->_pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, pc);


//     cmd.bindVertexBuffers(0, planet->_vertexBuffer, {0});
//     cmd.bindIndexBuffer(planet->_indexBuffer, 0, vk::IndexType::eUint32);
//     cmd.drawIndexed(planet->_indexCount, 1, 0, 0, 0);

//     return StageResult::Continue();
// }

// Planet::Planet(Raindrop::Engine& engine, std::shared_ptr<Raindrop::Render::IRenderOutput> output){
//     _output = output;
//     auto& modules = engine.getModuleManager();

//     _core = modules.getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
//     // if (!_core){

//     // }
//     _assets = modules.getModuleAs<Raindrop::Asset::AssetModule>("Asset");

//     _fragmentShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.frag.glsl");
//     _vertexShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.vert.glsl");

//     auto result = _fragmentShader->load().and_then([&]{return _vertexShader->load();});

//     if (!result) spdlog::info("{}", result.error().message());

//     _maxInstanceCount = 10000;

//     createPipelineLayout();
//     createPipeline();
//     createGridMesh(32, 32);
//     createInstanceBuffer();
// }

// Planet::~Planet(){
//     auto device = _core->device();
//     auto allocator = _core->allocator();

//     if (_pipeline){
//         device.destroyPipeline(_pipeline);    
//     }

//     if (_pipelineLayout){
//         device.destroyPipelineLayout(_pipelineLayout);
//     }

//     if (_vertexBuffer){
//         vmaDestroyBuffer(allocator, _vertexBuffer, _vertexAllocation);
//     }

//     if (_indexBuffer){
//         vmaDestroyBuffer(allocator, _indexBuffer, _indexAllocation);
//     }

//     if (_instanceBuffer){
//         vmaDestroyBuffer(allocator, _instanceBuffer, _instanceAllocation);
//     }
// }

// void Planet::createPipelineLayout(){

//     vk::PushConstantRange range(
//         vk::ShaderStageFlagBits::eVertex,
//         0,
//         sizeof(PushConstant)
//     );

//     vk::PipelineLayoutCreateInfo info(
//         {},
//         0, nullptr,
//         1, &range
//     );

//     auto device = _core->device();

//     if (auto result = device.createPipelineLayout(info); result.result != vk::Result::eSuccess){
//         throw std::runtime_error("Failed to create pipeline layout");
//     } else {
//         _pipelineLayout = result.value;
//     }
// }


// void Planet::createPipeline(){

//     vk::PipelineShaderStageCreateInfo stages[] = {
//         _vertexShader->stageInfo(),
//         _fragmentShader->stageInfo()
//     };

//     vk::VertexInputBindingDescription inputBinding{
//         0,
//         sizeof(Vertex),
//         vk::VertexInputRate::eVertex
//     };

//     vk::VertexInputAttributeDescription inputAttributes[] = {
//         vk::VertexInputAttributeDescription{ // Position
//             0,
//             0,
//             vk::Format::eR32G32B32Sfloat,
//             0
//         },
//         vk::VertexInputAttributeDescription{ // UV
//             1,
//             0,
//             vk::Format::eR32G32Sfloat,
//             0
//         }
//     };

//     vk::PipelineVertexInputStateCreateInfo inputStage(
//         {},
//         inputBinding,
//         inputAttributes
//     );

//     vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
//         {},
//         vk::PrimitiveTopology::eTriangleList,
//         vk::False
//     };

//     vk::PipelineViewportStateCreateInfo viewportStateInfo{
//         {},
//         1, nullptr,
//         1, nullptr
//     };

//     vk::PipelineRasterizationStateCreateInfo rasterInfo{
//         {},
//         vk::False,
//         vk::False,
//         vk::PolygonMode::eFill,
//         vk::CullModeFlagBits::eNone,
//         vk::FrontFace::eCounterClockwise,
//         vk::True,
//         0.f,
//         0.f,
//         0.f,
//         1.f
//     };

//     vk::PipelineMultisampleStateCreateInfo multisampleInfo{
//         {},
//         vk::SampleCountFlagBits::e1,
//         vk::False,
//         0.f,
//         nullptr,
//         vk::False,
//         vk::False
//     };

//     vk::PipelineDepthStencilStateCreateInfo depthStencilInfo{
//         {},
//         vk::True,
//         vk::True,
//         vk::CompareOp::eLess,
//         vk::False,
//         vk::False,
//         {},
//         {},
//         0.f,
//         1.f
//     };

//     vk::PipelineColorBlendAttachmentState attachments[] = {
//         vk::PipelineColorBlendAttachmentState{
//             vk::False,
//             vk::BlendFactor::eOne,
//             vk::BlendFactor::eZero,
//             vk::BlendOp::eAdd,
//             vk::BlendFactor::eOne,
//             vk::BlendFactor::eZero,
//             vk::BlendOp::eAdd,
//             {
//                 vk::ColorComponentFlagBits::eR |
//                 vk::ColorComponentFlagBits::eG |
//                 vk::ColorComponentFlagBits::eB |
//                 vk::ColorComponentFlagBits::eA
//             }
//         }
//     };

//     vk::PipelineColorBlendStateCreateInfo colorBlendInfo(
//         {},
//         vk::False,
//         vk::LogicOp::eCopy,
//         attachments,
//         {}
//     );

//     vk::DynamicState dynamicStates[] = {
//         vk::DynamicState::eViewport,
//         vk::DynamicState::eScissor
//     };

//     vk::PipelineDynamicStateCreateInfo dynamicStateInfo(
//         {},
//         dynamicStates
//     );

//     vk::GraphicsPipelineCreateInfo info(
//         {},
//         stages,
//         &inputStage,
//         &inputAssembly,
//         nullptr,
//         &viewportStateInfo,
//         &rasterInfo,
//         &multisampleInfo,
//         &depthStencilInfo,
//         &colorBlendInfo,
//         &dynamicStateInfo,
//         _pipelineLayout,
//         _output->renderPass(),
//         0,
//         {},
//         {}
//     );

//     auto device = _core->device();

//     if (auto result = device.createGraphicsPipeline({}, info); result.result != vk::Result::eSuccess){
//         throw std::runtime_error("Failed to create pipeline");
//     } else {
//         _pipeline = result.value;
//     }
// }

// void Planet::createGridMesh(uint32_t xcells, uint32_t ycells){
//     std::vector<Vertex> vertices;
//     vertices.reserve((xcells + 1) * (ycells + 1));

//     for (uint32_t y = 0; y <= ycells; ++y) {
//         for (uint32_t x = 0; x <= xcells; ++x) {
//             Vertex v{
//                 .position = glm::vec3(
//                     static_cast<float>(x) * (1.f / float(xcells)) - 0.5f,
//                     static_cast<float>(y) * (1.f / float(xcells)) - 0.5f,
//                     0.0f
//                 ),

//                 .UV = glm::vec2(
//                     static_cast<float>(x) / static_cast<float>(xcells),
//                     static_cast<float>(y) / static_cast<float>(ycells)
//                 )
//             };

//             vertices.push_back(v);
//         }
//     }

//     std::vector<uint32_t> indices;
//     indices.reserve(xcells * ycells * 6);

//     for (uint32_t y = 0; y < ycells; ++y) {
//         for (uint32_t x = 0; x < xcells; ++x) {
//             uint32_t i0 = y * (xcells + 1) + x;
//             uint32_t i1 = i0 + 1;
//             uint32_t i2 = i0 + (xcells + 1);
//             uint32_t i3 = i2 + 1;

//             // triangle 1
//             indices.push_back(i0);
//             indices.push_back(i2);
//             indices.push_back(i1);

//             // triangle 2
//             indices.push_back(i1);
//             indices.push_back(i2);
//             indices.push_back(i3);
//         }
//     }

//     auto allocator = _core->allocator();

//     /*
//     vk::Buffer stagingBuffer;
//     VmaAllocation staginAllocation;

//     {
//         vk::BufferCreateInfo info(
//             {},
//             static_cast<vk::DeviceSize>(vertices.size() * sizeof(Vertex)),
//             vk::BufferUsageFlagBits::eVertexBuffer,
//             vk::SharingMode::eExclusive,
//             {}, {}
//         );

//         VmaAllocationCreateInfo allocationInfo{
//             {},
//             VMA_MEMORY_USAGE_GPU_ONLY,
//             0,
//             0,
//             0,
//             nullptr,
//             nullptr,
//             0.1f
//         };

//         VkBuffer buffer;
        
//         vmaCreateBuffer(
//             allocator,
//             reinterpret_cast<VkBufferCreateInfo*>(&info),
//             &allocationInfo,
//             &buffer,
//             &staginAllocation,
//             nullptr
//         );

//         stagingBuffer = buffer;
//     }
//     */


//     // vertex buffer
//     {
//         size_t size = vertices.size() * sizeof(Vertex);


//         vk::BufferCreateInfo info(
//             {},
//             static_cast<vk::DeviceSize>(size),
//             vk::BufferUsageFlagBits::eVertexBuffer,
//             vk::SharingMode::eExclusive,
//             {}, {}
//         );

//         VmaAllocationCreateInfo allocationInfo{
//             {},
//             VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer. I don't want to make staging system right now
//             0,
//             0,
//             0,
//             nullptr,
//             nullptr,
//             0.1f
//         };

//         VkBuffer buffer;
        
//         vmaCreateBuffer(
//             allocator,
//             reinterpret_cast<VkBufferCreateInfo*>(&info),
//             &allocationInfo,
//             &buffer,
//             &_vertexAllocation,
//             nullptr
//         );

//         _vertexBuffer = buffer;

//         void* data;

//         vmaMapMemory(
//             allocator,
//             _vertexAllocation,
//             &data
//         );

//         std::memcpy(data, vertices.data(), size);

//         vmaUnmapMemory(
//             allocator,
//             _vertexAllocation
//         );

//         vmaFlushAllocation(
//             allocator,
//             _vertexAllocation,
//             0,
//             size
//         );
//     }

//     {
//         size_t size = indices.size() * sizeof(uint32_t);

//         vk::BufferCreateInfo info(
//             {},
//             static_cast<vk::DeviceSize>(size),
//             vk::BufferUsageFlagBits::eIndexBuffer,
//             vk::SharingMode::eExclusive,
//             {}, {}
//         );

//         _indexCount = indices.size();

//         VmaAllocationCreateInfo allocationInfo{
//             {},
//             VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer. I don't want to make staging system right now
//             0,
//             0,
//             0,
//             nullptr,
//             nullptr,
//             0.1f
//         };

//         VkBuffer buffer;
        
//         vmaCreateBuffer(
//             allocator,
//             reinterpret_cast<VkBufferCreateInfo*>(&info),
//             &allocationInfo,
//             &buffer,
//             &_indexAllocation,
//             nullptr
//         );

//         _indexBuffer = buffer;

//         void* data;

//         vmaMapMemory(
//             allocator,
//             _indexAllocation,
//             &data
//         );

//         std::memcpy(data, indices.data(), size);

//         vmaUnmapMemory(
//             allocator,
//             _indexAllocation
//         );

//         vmaFlushAllocation(
//             allocator,
//             _indexAllocation,
//             0,
//             size
//         );
//     }
// }

// void Planet::createInstanceBuffer(){
    
//     auto allocator = _core->allocator();
//     size_t size = _maxInstanceCount * sizeof(Instance);

//     vk::BufferCreateInfo info(
//         {},
//         static_cast<vk::DeviceSize>(size),
//         vk::BufferUsageFlagBits::eVertexBuffer,
//         vk::SharingMode::eExclusive,
//         {}, {}
//     );

//     VmaAllocationCreateInfo allocationInfo{
//         {},
//         VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer. I don't want to make staging system right now
//         0,
//         0,
//         0,
//         nullptr,
//         nullptr,
//         0.1f
//     };

//     VkBuffer buffer;
    
//     vmaCreateBuffer(
//         allocator,
//         reinterpret_cast<VkBufferCreateInfo*>(&info),
//         &allocationInfo,
//         &buffer,
//         &_instanceAllocation,
//         nullptr
//     );

//     _instanceBuffer = buffer;
// }


