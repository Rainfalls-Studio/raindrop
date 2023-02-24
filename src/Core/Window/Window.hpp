#ifndef __RAINDROP_CORE_WINDOW_WINDOW_HPP__
#define __RAINDROP_CORE_WINDOW_WINDOW_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../String.hpp"
#include "../Maths/Maths.hpp"
#include "../IO/DLLReader.hpp"

namespace Raindrop::Core::Window{
	class Window{
		public:
			Window(const char* filepath);
			~Window();

			bool loaded() const;

			void setTitle(const String& title);
			void setTitle(const char* title);
			void setSize(const Maths::vec2<uint32>& size);
			void setPosition(const Maths::vec2<uint32>& position);

			const char* getTitle() const;
			Maths::vec2<uint32> getSize() const;
			Maths::vec2<uint32> getPosition() const;

			const char* getAPI() const;
		
		private:
			using InitFnc = __stdcall bool(*)(void);
			using ShutdownFnc = __stdcall void(*)(void);
			using SetTitleFnc = __stdcall void(*)(const char*);
			using SetSizeFnc = __stdcall void(*)(Maths::vec2<uint32>);
			using SetPositionFnc = __stdcall void(*)(Maths::vec2<uint32>);
			using GetTitleFnc = __stdcall const char*(*)(void);
			using GetSizeFnc = __stdcall Maths::vec2<uint32>(*)(void);
			using GetPositionFnc = __stdcall Maths::vec2<uint32>(*)(void);
			using GetAPIFnc = __stdcall const char*(*)(void);

			void read(const char* filepath);

			bool _loaded;
			IO::DLLReader _dll;

			InitFnc _initFnc;
			ShutdownFnc _shutdownFnc;
			SetTitleFnc _setTitleFnc;
			SetSizeFnc _setSizeFnc;
			SetPositionFnc _setPositionFnc;
			GetTitleFnc _getTitleFnc;
			GetSizeFnc _getSizeFnc;
			GetPositionFnc _getPositionFnc;
			GetAPIFnc _getAPIFnc;
	};
}

#endif