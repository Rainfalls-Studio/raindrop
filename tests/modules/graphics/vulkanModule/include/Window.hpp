#ifndef __VULKANMODULE_WINDOW_HPP__
#define __VULKANMODULE_WINDOW_HPP__

#include "core.hpp"

class Window : public Raindrop::Graphics::Window{
	public:
		Window();
		~Window();

		virtual void setTitle(const char* title) override;
		virtual void setSize(Raindrop::Core::Maths::vec2<Raindrop::uint32> size) override;
		virtual void setPosition(Raindrop::Core::Maths::vec2<Raindrop::uint32> position) override;
		virtual const char* getTitle() const override;
		virtual Raindrop::Core::Maths::vec2<Raindrop::uint32> getSize() const override;
		virtual Raindrop::Core::Maths::vec2<Raindrop::uint32> getPosition() const override;
		virtual const char* getAPI() const override;
		virtual bool loaded() const override;

		SDL_Window* getWindow() const;

		VkSurfaceKHR createSurface(VkInstance instance);


	private:
		static Raindrop::Core::Debug::Log::Severity SDL_to_raindrop_severity(SDL_LogPriority priority);
		static void SDL_log(void* userData, int category, SDL_LogPriority priority, const char* message);
		SDL_Window* window;
};

#endif