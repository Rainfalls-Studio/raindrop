#include "core.hpp"
#include "render/context/vulkan/AllocationCallback.hpp"

namespace rnd::render::context::vulkan{
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
