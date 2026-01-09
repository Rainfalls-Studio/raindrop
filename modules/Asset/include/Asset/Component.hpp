#pragma once

#include "Asset.hpp"

namespace Asset{
	class Component{
		public:
			Component() = default;
			virtual ~Component() = default;

			virtual void onUpdate(const Asset& asset) = 0;
			virtual void onCleanup(const Asset& asset) = 0;
	};
}