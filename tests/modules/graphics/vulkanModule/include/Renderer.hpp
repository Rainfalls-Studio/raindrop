#ifndef __VULKANMODULE_RENDERER_HPP__
#define __VULKANMODULE_RENDERER_HPP__

#include "core.hpp"
#include "Allocator.hpp"
#include "device/Instance.hpp"
#include "device/PhysicalDevice.hpp"
#include "device/Device.hpp"

class Renderer : public gfx::Renderer{
	public:
		Renderer(Allocator& allocator);
		~Renderer();

		virtual gfx::Context* createContext(Allocator& allocator, gfx::Window& window) override;

		virtual const char* getAPI() const override;
		virtual bool loaded() const override;
	
	private:
		GfxAllocator _allocator;
		Instance _instance;
		Array<PhysicalDevice*> _physicalDevices;
		Device _device;

		bool _loaded;
};

#endif