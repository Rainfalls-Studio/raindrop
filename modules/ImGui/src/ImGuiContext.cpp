#include "ImGui/ImGuiContext.hpp"
#include "ImGui/backend/imgui_impl_sdl3.h"

#include <RenderOutput/WindowRenderOutput.hpp>
#include <Event/MouseEvent.hpp>
#include <Event/KeyboardEvent.hpp>
#include <Window/WindowEvents.hpp>

#include <imgui_impl_vulkan.h>
#include <spdlog/spdlog.h>

namespace Raindrop::ImGui{

    ImGuiKey ImGui_Raindrop_KeyEventToImGuiKey(Event::Key keycode, Event::PhysicalKey scancode){
        using Event::Key;
        using Event::PhysicalKey;


        // Keypad doesn't have individual key values in SDL3
        switch (scancode){
            case PhysicalKey::KP_0: return ImGuiKey_Keypad0;
            case PhysicalKey::KP_1: return ImGuiKey_Keypad1;
            case PhysicalKey::KP_2: return ImGuiKey_Keypad2;
            case PhysicalKey::KP_3: return ImGuiKey_Keypad3;
            case PhysicalKey::KP_4: return ImGuiKey_Keypad4;
            case PhysicalKey::KP_5: return ImGuiKey_Keypad5;
            case PhysicalKey::KP_6: return ImGuiKey_Keypad6;
            case PhysicalKey::KP_7: return ImGuiKey_Keypad7;
            case PhysicalKey::KP_8: return ImGuiKey_Keypad8;
            case PhysicalKey::KP_9: return ImGuiKey_Keypad9;
            case PhysicalKey::KP_PERIOD: return ImGuiKey_KeypadDecimal;
            case PhysicalKey::KP_DIVIDE: return ImGuiKey_KeypadDivide;
            case PhysicalKey::KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
            case PhysicalKey::KP_MINUS: return ImGuiKey_KeypadSubtract;
            case PhysicalKey::KP_PLUS: return ImGuiKey_KeypadAdd;
            case PhysicalKey::KP_ENTER: return ImGuiKey_KeypadEnter;
            case PhysicalKey::KP_EQUALS: return ImGuiKey_KeypadEqual;
            default: break;
        }

        switch (keycode){
            case Key::TAB: return ImGuiKey_Tab;
            case Key::LEFT: return ImGuiKey_LeftArrow;
            case Key::RIGHT: return ImGuiKey_RightArrow;
            case Key::UP: return ImGuiKey_UpArrow;
            case Key::DOWN: return ImGuiKey_DownArrow;
            case Key::PAGEUP: return ImGuiKey_PageUp;
            case Key::PAGEDOWN: return ImGuiKey_PageDown;
            case Key::HOME: return ImGuiKey_Home;
            case Key::END: return ImGuiKey_End;
            case Key::INSERT: return ImGuiKey_Insert;
            case Key::DELETE: return ImGuiKey_Delete;
            case Key::BACKSPACE: return ImGuiKey_Backspace;
            case Key::SPACE: return ImGuiKey_Space;
            case Key::RETURN: return ImGuiKey_Enter;
            case Key::ESCAPE: return ImGuiKey_Escape;
            //case Key::APOSTROPHE: return ImGuiKey_Apostrophe;
            case Key::COMMA: return ImGuiKey_Comma;
            //case Key::MINUS: return ImGuiKey_Minus;
            case Key::PERIOD: return ImGuiKey_Period;
            //case Key::SLASH: return ImGuiKey_Slash;
            case Key::SEMICOLON: return ImGuiKey_Semicolon;
            //case Key::EQUALS: return ImGuiKey_Equal;
            //case Key::LEFTBRACKET: return ImGuiKey_LeftBracket;
            //case Key::BACKSLASH: return ImGuiKey_Backslash;
            //case Key::RIGHTBRACKET: return ImGuiKey_RightBracket;
            //case Key::GRAVE: return ImGuiKey_GraveAccent;
            case Key::CAPSLOCK: return ImGuiKey_CapsLock;
            case Key::SCROLLLOCK: return ImGuiKey_ScrollLock;
            case Key::NUMLOCKCLEAR: return ImGuiKey_NumLock;
            case Key::PRINTSCREEN: return ImGuiKey_PrintScreen;
            case Key::PAUSE: return ImGuiKey_Pause;
            case Key::LCTRL: return ImGuiKey_LeftCtrl;
            case Key::LSHIFT: return ImGuiKey_LeftShift;
            case Key::LALT: return ImGuiKey_LeftAlt;
            case Key::LGUI: return ImGuiKey_LeftSuper;
            case Key::RCTRL: return ImGuiKey_RightCtrl;
            case Key::RSHIFT: return ImGuiKey_RightShift;
            case Key::RALT: return ImGuiKey_RightAlt;
            case Key::RGUI: return ImGuiKey_RightSuper;
            case Key::APPLICATION: return ImGuiKey_Menu;
            case Key::NUM_0: return ImGuiKey_0;
            case Key::NUM_1: return ImGuiKey_1;
            case Key::NUM_2: return ImGuiKey_2;
            case Key::NUM_3: return ImGuiKey_3;
            case Key::NUM_4: return ImGuiKey_4;
            case Key::NUM_5: return ImGuiKey_5;
            case Key::NUM_6: return ImGuiKey_6;
            case Key::NUM_7: return ImGuiKey_7;
            case Key::NUM_8: return ImGuiKey_8;
            case Key::NUM_9: return ImGuiKey_9;
            case Key::A: return ImGuiKey_A;
            case Key::B: return ImGuiKey_B;
            case Key::C: return ImGuiKey_C;
            case Key::D: return ImGuiKey_D;
            case Key::E: return ImGuiKey_E;
            case Key::F: return ImGuiKey_F;
            case Key::G: return ImGuiKey_G;
            case Key::H: return ImGuiKey_H;
            case Key::I: return ImGuiKey_I;
            case Key::J: return ImGuiKey_J;
            case Key::K: return ImGuiKey_K;
            case Key::L: return ImGuiKey_L;
            case Key::M: return ImGuiKey_M;
            case Key::N: return ImGuiKey_N;
            case Key::O: return ImGuiKey_O;
            case Key::P: return ImGuiKey_P;
            case Key::Q: return ImGuiKey_Q;
            case Key::R: return ImGuiKey_R;
            case Key::S: return ImGuiKey_S;
            case Key::T: return ImGuiKey_T;
            case Key::U: return ImGuiKey_U;
            case Key::V: return ImGuiKey_V;
            case Key::W: return ImGuiKey_W;
            case Key::X: return ImGuiKey_X;
            case Key::Y: return ImGuiKey_Y;
            case Key::Z: return ImGuiKey_Z;
            case Key::F1: return ImGuiKey_F1;
            case Key::F2: return ImGuiKey_F2;
            case Key::F3: return ImGuiKey_F3;
            case Key::F4: return ImGuiKey_F4;
            case Key::F5: return ImGuiKey_F5;
            case Key::F6: return ImGuiKey_F6;
            case Key::F7: return ImGuiKey_F7;
            case Key::F8: return ImGuiKey_F8;
            case Key::F9: return ImGuiKey_F9;
            case Key::F10: return ImGuiKey_F10;
            case Key::F11: return ImGuiKey_F11;
            case Key::F12: return ImGuiKey_F12;
            case Key::F13: return ImGuiKey_F13;
            case Key::F14: return ImGuiKey_F14;
            case Key::F15: return ImGuiKey_F15;
            case Key::F16: return ImGuiKey_F16;
            case Key::F17: return ImGuiKey_F17;
            case Key::F18: return ImGuiKey_F18;
            case Key::F19: return ImGuiKey_F19;
            case Key::F20: return ImGuiKey_F20;
            case Key::F21: return ImGuiKey_F21;
            case Key::F22: return ImGuiKey_F22;
            case Key::F23: return ImGuiKey_F23;
            case Key::F24: return ImGuiKey_F24;
            case Key::AC_BACK: return ImGuiKey_AppBack;
            case Key::AC_FORWARD: return ImGuiKey_AppForward;
            default: break;
        }

        // Fallback to scancode
        switch (scancode){
            case PhysicalKey::GRAVE: return ImGuiKey_GraveAccent;
            case PhysicalKey::MINUS: return ImGuiKey_Minus;
            case PhysicalKey::EQUALS: return ImGuiKey_Equal;
            case PhysicalKey::LEFTBRACKET: return ImGuiKey_LeftBracket;
            case PhysicalKey::RIGHTBRACKET: return ImGuiKey_RightBracket;
            // case PhysicalKey::NONUSBACKSLASH: return ImGuiKey_Oem102;
            case PhysicalKey::BACKSLASH: return ImGuiKey_Backslash;
            case PhysicalKey::SEMICOLON: return ImGuiKey_Semicolon;
            case PhysicalKey::APOSTROPHE: return ImGuiKey_Apostrophe;
            case PhysicalKey::COMMA: return ImGuiKey_Comma;
            case PhysicalKey::PERIOD: return ImGuiKey_Period;
            case PhysicalKey::SLASH: return ImGuiKey_Slash;
            default: break;
        }

        return ImGuiKey_None;
    }

    ImGuiContext::ImGuiContext(std::shared_ptr<Render::RenderCoreModule> core, std::shared_ptr<Render::IRenderOutput> output) : 
        _core{core},
        _output{output}
    {
        _ctx = ::ImGui::CreateContext();
        ::ImGui::SetCurrentContext(_ctx);

        try{
            setupBackend();
        } catch (const std::exception& e){
            spdlog::error("Failed to setup ImGui backend for render target : {}", e.what());

            ::ImGui::DestroyContext(_ctx);
            ::ImGui::SetCurrentContext(nullptr);

            throw std::runtime_error("Failed to setup ImGui context");
        }
    }

    ImGuiContext::~ImGuiContext(){
        ::ImGui::SetCurrentContext(_ctx);
        ImGui_ImplVulkan_DestroyFontsTexture();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ::ImGui::DestroyContext();

        destroyDescriptorPool();
    }

    vk::DescriptorPool ImGuiContext::createDescriptorPool(){
        auto device = _core->deviceManager().device();

        vk::DescriptorPoolSize sizes[] = {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

        vk::DescriptorPoolCreateInfo info{
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            1000,
            sizeof(sizes) / sizeof(sizes[0]),
            sizes
        };

        auto result = device.createDescriptorPool(info);
        if (result.result != vk::Result::eSuccess){
            spdlog::error("Failed to create descriptor pool : {}", vk::to_string(result.result));
            return nullptr;
        }
        
        return result.value;
    }

    void ImGuiContext::destroyDescriptorPool(){
        auto device = _core->deviceManager().device();

        device.destroyDescriptorPool(_pool);
    }


    void ImGuiContext::setupBackend(){
        auto windowOutput = std::dynamic_pointer_cast<Render::WindowRenderOutput>(_output);
        
        if (!windowOutput){
            spdlog::error("The render output is not a window ! Other form of render output are not supported !");
            throw std::runtime_error("Only window render outputs are supported for now !");
        }

        auto window = windowOutput->window().lock();

        if (!window){
            spdlog::error("The window is not valid !");
            throw std::runtime_error("The window is not valid !");
        }

        vk::RenderPass renderPass = windowOutput->renderPass();
        if (!renderPass){
            spdlog::error("The output target doesn't have a render pass !");
            throw std::runtime_error("Missing render pass");
        }

        if (!ImGui_ImplSDL3_InitForVulkan(window->getWindow())){
            spdlog::error("Failed to initialize SDL3 ImGui implementation : {}", SDL_GetError());
            throw std::runtime_error("Failed to initialize SDL3 implementation");
        }

        _pool = createDescriptorPool();

        if (!_pool){
            ImGui_ImplSDL3_Shutdown();
            throw std::runtime_error("Failed to create descriptor pool");
        }

        ImGui_ImplVulkan_InitInfo info{};

        auto& deviceManager = _core->deviceManager();
        
        info.Device = deviceManager.device();
        info.PhysicalDevice = deviceManager.physicalDevice();
        info.Instance = deviceManager.instance();
        
        auto graphicsQueue = _core->graphicsQueue();
        info.Queue = *graphicsQueue;
        info.QueueFamily = graphicsQueue.familyIndex();

        info.DescriptorPool = _pool;

        info.RenderPass = _output->renderPass();
        info.ImageCount = windowOutput->getBufferCount();
        info.MinImageCount = 2;
        info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        // info.UseDynamicRendering = true;

        if (!ImGui_ImplVulkan_Init(&info)){
            spdlog::error("Failed to initialize Vulkan ImGui implementation !");

            destroyDescriptorPool();
            ImGui_ImplSDL3_Shutdown();

            throw std::runtime_error("Failed to initialize Vulkan implementation");
        }

        if (!ImGui_ImplVulkan_CreateFontsTexture()){
            spdlog::error("Failed to create vulkan fonts textures");

            destroyDescriptorPool();
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplVulkan_Shutdown();

            throw std::runtime_error("Failed to create Vulkan fonts textures");
        }


        ::ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }


    void ImGuiContext::begin(){
        std::lock_guard lock(_mtx);
        
        ::ImGui::SetCurrentContext(_ctx);
        
        // update events
        std::swap(_frontEventCaptures, _backEventCaptures);
        auto& io = ::ImGui::GetIO();
        for (const auto& event : _frontEventCaptures.events){
            event(io);
        }

        _frontEventCaptures.events.clear();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ::ImGui::NewFrame();

        float scale = 1.f;// _output->scale();

        ImGuiStyle& style = ::ImGui::GetStyle();
        ImGuiStyle styleold = style; // Backup colors
        style = ImGuiStyle(); // IMPORTANT: ScaleAllSizes will change the original size, so we should reset all style config
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize  = 1.0f;
        style.PopupBorderSize  = 1.0f;
        style.FrameBorderSize  = 1.0f;
        style.TabBorderSize    = 1.0f;
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;
        style.ScaleAllSizes(scale);
        std::memcpy(style.Colors, styleold.Colors, sizeof(style.Colors));

        io.FontGlobalScale = scale;

        ::ImGui::ShowDemoWindow();
    }

    void ImGuiContext::end(){
        std::lock_guard lock(_mtx);
        ::ImGui::SetCurrentContext(_ctx);
        ::ImGui::EndFrame();
    }

    void ImGuiContext::render(vk::CommandBuffer cmdBuffer){
        std::lock_guard lock(_mtx);
        ::ImGui::SetCurrentContext(_ctx);

        ::ImGui::Render();
        auto drawData = ::ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, cmdBuffer);
    }

    void ImGuiContext::setLayer(const std::shared_ptr<Layers::Layer>& layer){
        _layer = layer->createChild("ImGui", 1.f);

        _eventSubscriber = layer->subscribe<Event::Event>(
            [this](const Event::Event& event) -> Layers::Result {
                if (handleEvent(event)){
                    return Layers::Result::Consume();
                }
                return Layers::Result::Continue();
            }
        );
    }

    bool ImGuiContext::handleEvent(const Event::Event& e){
        if (const Event::MouseEvent* mouseEvent = dynamic_cast<const Event::MouseEvent*>(&e)){
            return handleMouseEvent(*mouseEvent);
        }
        else if (const Event::KeyboardEvent* keyboardEvent = dynamic_cast<const Event::KeyboardEvent*>(&e)){
            return handleKeyboardEvent(*keyboardEvent);
        }
        else if (const Window::Events::WindowEvent* windowEvent = dynamic_cast<const Window::Events::WindowEvent*>(&e)){
            return handleWindowEvent(*windowEvent);
        }

        return false;
    }

    bool ImGuiContext::handleWindowEvent(const Window::Events::WindowEvent& e){
        using namespace Window::Events;

        if (const WindowMouseEntered* mouseEnteredEvent = dynamic_cast<const WindowMouseEntered*>(&e)){
            const auto event = *mouseEnteredEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO&){

                ImGui_ImplSDL3_Data* bd = ImGui_ImplSDL3_GetBackendData();

                bd->MousePendingLeaveFrame = 0;
                // bd->MouseWindowID = event->window.windowID;
            });
        }
        else if (const WindowMouseLeaved* mouseLeavedEvent = dynamic_cast<const WindowMouseLeaved*>(&e)){
            const auto event = *mouseLeavedEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO&){

                ImGui_ImplSDL3_Data* bd = ImGui_ImplSDL3_GetBackendData();
                bd->MousePendingLeaveFrame = ::ImGui::GetFrameCount() + 1;
                // bd->MouseWindowID = event->window.windowID;
            });
        }
        else if (const WindowFocusGained* focusGainedEvent = dynamic_cast<const WindowFocusGained*>(&e)){
            const auto event = *focusGainedEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddFocusEvent(true);
            });
        }
        else if (const WindowFocusLost* focusLostEvent = dynamic_cast<const WindowFocusLost*>(&e)){
            const auto event = *focusLostEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddFocusEvent(false);
            });
        }
        return false;
    }


    bool ImGuiContext::handleMouseEvent(const Event::MouseEvent& e){
        
        // I know chained if are not good, but it's only events

        if (const Event::MouseButtonEvent* buttonEvent = dynamic_cast<const Event::MouseButtonEvent*>(&e)){
            _backEventCaptures.events.push_back([event=*buttonEvent](::ImGuiIO& io) {
                int button = -1;

                ImGui_ImplSDL3_Data* bd = ImGui_ImplSDL3_GetBackendData();

                switch (event.button()){
                    case Event::MouseButton::LEFT: button=0; break;
                    case Event::MouseButton::RIGHT: button=1; break;
                    case Event::MouseButton::MIDDLE: button=2; break;
                    case Event::MouseButton::X1: button=3; break;
                    case Event::MouseButton::X2: button=4; break;
                }

                if (button == -1) return;

                //io.AddMouseSourceEvent(event->button.which == SDL_TOUCH_MOUSEID ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
                io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
                io.AddMouseButtonEvent(button, event.down());

                bd->MouseButtonsDown = (event.down()) ? (bd->MouseButtonsDown | (1 << button)) : (bd->MouseButtonsDown & ~(1 << button));
            });
        }
        else if (const Event::MouseMovedEvent* movedEvent = dynamic_cast<const Event::MouseMovedEvent*>(&e)){
            const auto event = *movedEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddMousePosEvent(event.position().x, event.position().y);
            });
        }
        else if (const Event::MouseWheelEvent* wheelEvent = dynamic_cast<const Event::MouseWheelEvent*>(&e)){
            const auto event = *wheelEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddMouseWheelEvent(event.wheel().x, event.wheel().y);
            });
        }

        return _backEventCaptures.wantCaptureMouse;
    }

    bool ImGuiContext::handleKeyboardEvent(const Event::KeyboardEvent& e){
        if (const Event::TextInputEvent* textEvent = dynamic_cast<const Event::TextInputEvent*>(&e)){
            const auto event = *textEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddInputCharactersUTF8(event.text().c_str());
            });
        }
        else if (const Event::KeyEvent* keyEvent = dynamic_cast<const Event::KeyEvent*>(&e)){
            const auto event = *keyEvent;
            _backEventCaptures.events.push_back([event](::ImGuiIO& io){
                io.AddKeyEvent(ImGui_Raindrop_KeyEventToImGuiKey(event.key(), event.physicalKey()), event.down());
            });
        }

        return _backEventCaptures.wantCaptureKeyboard;
    }

    // ------------------------------------------------------------------------------------

    BeginStage::BeginStage(std::shared_ptr<ImGuiContext> context) : _ctx(context){}

    const char* BeginStage::name() const{
        return "ImGui - Begin Stage";
    }

    void BeginStage::shutdown() {}

    Scheduler::StageResult BeginStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        context->begin();
        return Scheduler::StageResult::Continue();
    }



    // -----------------------------------------------------------------------------------




    EndStage::EndStage(std::shared_ptr<ImGuiContext> context) : _ctx(context){}

    const char* EndStage::name() const{
        return "ImGui - End Stage";
    }

    void EndStage::shutdown() {}

    Scheduler::StageResult EndStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        context->end();

        return Scheduler::StageResult::Continue();
    }



    // -----------------------------------------------------------------------------------

    

    RenderStage::RenderStage(std::shared_ptr<ImGuiContext> context, std::shared_ptr<Render::RenderCommandContext> cmdCtx) : 
        _ctx{context},
        _cmdCtx(cmdCtx)
    {}

    const char* RenderStage::name() const{
        return "ImGui - Render Stage";
    }

    void RenderStage::shutdown(){}
    Scheduler::StageResult RenderStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        auto cmdCtx = _cmdCtx.lock();

        if (!cmdCtx){
            return Scheduler::StageResult::Skip("No render command context");
        }

        vk::CommandBuffer cmdBuffer = cmdCtx->currentBuffer().cmdBuffer;
        context->render(cmdBuffer);

        return Scheduler::StageResult::Continue();
    }
}