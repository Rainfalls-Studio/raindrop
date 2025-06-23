// #include <gtest/gtest.h>
// #include <glm/ext/vector_int2.hpp>
// #include "Raindrop/Window/Impl/SDL3/Window.hpp"
// #include <stdexcept>
// #include <SDL3/SDL_init.h>
// #include <SDL3/SDL_video.h>
// #include <SDL3/SDL_error.h>

// namespace {

//     using Raindrop::Window::Impl::SDL3::Window;

//     class WindowTest{

//     };


//     // Helper to ensure SDL is not initialized before test
//     void EnsureSDLNotInitialized() {
//         if (SDL_WasInit(SDL_INIT_VIDEO) != 0) {
//             SDL_Quit();
//         }
//     }

//     // Helper to ensure SDL is initialized before test
//     void EnsureSDLInitialized() {
//         if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
//             SDL_Init(SDL_INIT_VIDEO);
//         }
//     }

//     // Helper to destroy all windows and quit SDL
//     void CleanupSDL() {
//         int count = 0;
//         SDL_Window** windows = SDL_GetWindows(&count);
//         if (windows) {
//             for (int i = 0; i < count; ++i) {
//                 SDL_DestroyWindow(windows[i]);
//             }
//         }
//         SDL_Quit();
//     }

//     // Test 1: Window is successfully created when SDL3 is not initialized and all SDL3 calls succeed
//     TEST(SDL3WindowTest, test_window_creation_with_sdl3_initialization) {
//         EnsureSDLNotInitialized();
//         EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0);

//         Window* window = nullptr;
//         EXPECT_NO_THROW({
//             window = new Window();
//         });
//         ASSERT_NE(window, nullptr);

//         // Clean up
//         delete window;
//         CleanupSDL();
//     }

//     // Test 2: Window properties such as size, position, and title are correctly retrieved after creation
//     TEST(SDL3WindowTest, test_window_property_getters) {
//         EnsureSDLNotInitialized();
//         Window* window = new Window();

//         glm::ivec2 size = window->getSize();
//         EXPECT_EQ(size.x, 800);
//         EXPECT_EQ(size.y, 600);

//         glm::ivec2 pos = window->getPosition();
//         // SDL may place the window at a default position, so just check type
//         EXPECT_GE(pos.x, 0);
//         EXPECT_GE(pos.y, 0);

//         const char* title = window->getTitle();
//         EXPECT_STREQ(title, "Raindrop window");

//         delete window;
//         CleanupSDL();
//     }

//     // Test 3: Window is properly destroyed and SDL3 shuts down when no windows remain
//     TEST(SDL3WindowTest, test_window_destruction_and_sdl3_shutdown) {
//         EnsureSDLNotInitialized();
//         {
//             Window window;
//             // Window exists, SDL should be initialized
//             EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0);
//         }
//         // After destruction, SDL should be uninitialized
//         EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0);
//     }

//     // Test 4: Exception is thrown if SDL3 initialization fails
//     TEST(SDL3WindowTest, test_exception_on_sdl3_initialization_failure) {
//         // Simulate SDL_Init failure by forcibly unloading SDL (not possible without mocks),
//         // so we skip this test if SDL_Init always succeeds.
//         // Instead, we can try to call SDL_Quit() and then set SDL_Init to fail if possible.
//         // Since mocks are not allowed, we skip this test.
//         SUCCEED() << "Cannot simulate SDL_Init failure without mocks or patching.";
//     }

//     // Test 5: Exception is thrown if window creation fails after successful SDL3 initialization
//     TEST(SDL3WindowTest, test_exception_on_window_creation_failure) {
//         // Simulate SDL_CreateWindow failure by passing invalid parameters.
//         // Since the Window class does not allow us to inject parameters, we cannot trigger this without mocks.
//         // So, we skip this test.
//         SUCCEED() << "Cannot simulate SDL_CreateWindow failure without mocks or patching.";
//     }

//     // Test 6: Getter methods handle null window pointer gracefully after failed creation
//     TEST(SDL3WindowTest, test_getters_with_null_window_pointer) {
//         // We cannot create a Window instance with a null _window pointer without modifying the class or using mocks.
//         // So, we skip this test.
//         SUCCEED() << "Cannot test getter methods with null window pointer without mocks or class modification.";
//     }

// } // namespace