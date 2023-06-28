#ifndef __RAINDROP_GRAPHICS_INSTANCE_HPP__
#define __RAINDROP_GRAPHICS_INSTANCE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Instance{
		public:
			static constexpr const char* APPLICATION_NAME = "Raindrop";
			static constexpr const char* ENGINE_NAME = "Raindrop Engine";
			static constexpr uint32_t APPLICATION_VERSION = VK_MAKE_API_VERSION(0, 0, 1, 0);
			static constexpr uint32_t ENGINE_VERSION = VK_MAKE_API_VERSION(0, 0, 1, 0);
			static constexpr uint32_t API_VERSION = VK_API_VERSION_1_3;

			static constexpr std::array<const char*, 0> REQUIRED_LAYERS = {};
			static constexpr std::array<const char*, 0> REQUIRED_EXTENSIONS = {};
			static constexpr std::array<const char*, 1> REQUIRED_VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

			#if defined(NDEBUG)
				static constexpr bool REQUIRE_VALIDATION = false;
			#else
				static constexpr bool REQUIRE_VALIDATION = true;
			#endif

			Instance(GraphicsContext& context);
			~Instance();

			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			VkInstance get() const;

		private:
			GraphicsContext& _context;
			VkInstance _instance = VK_NULL_HANDLE;

			std::vector<const char*> _requiredExtensions;

			void build();
			void checkExtensions();
			void checkLayers();
			void checkValidationLayers();
			void setupDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& info);
			void requireGraphicsExtensions();

	};
}

#endif