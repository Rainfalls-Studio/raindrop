#include <Core/Time/Time.hpp>

#if defined(RAINDROP_WINDOWS)
	#include <windows.h>
#endif

namespace Raindrop::Core::Time{
	#if defined(RAINDROP_WINDOWS)
		RAINDROP_API void sleep(tick time){
			::Sleep(static_cast<DWORD>(time));
		}
	#endif

	#if defined(RAINDROP_WINDOWS)
		RAINDROP_API tick ticks(){
			return static_cast<tick>(::GetTickCount64());
		}
	#endif
}