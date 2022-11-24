#include "core.hpp"
#include "render/vulkan/AllocationCallback.hpp"

namespace rnd::render::vulkan{
	void AllocationCallback::init(){
		PROFILE_FUNCTION();
		callback.pUserData = nullptr;
		callback.pfnAllocation = nullptr;
		callback.pfnReallocation = nullptr;
		callback.pfnFree = nullptr;
		callback.pfnInternalAllocation = nullptr;
		callback.pfnInternalFree = nullptr;
	}
}
