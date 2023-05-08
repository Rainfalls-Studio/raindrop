#ifndef __RAINDROP_GRAPHCICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHCICS_CONTEXT_HPP__

#include <Graphics/common.hpp>
#include <Core/Maths/Maths.hpp>
#include <Core/Scene/Scene.hpp>

namespace Raindrop::Graphics{
	class RAINDROP_API Context{
		public:
			Context() = default;
			virtual ~Context() = default;

			virtual bool begin() = 0;
			virtual void end() = 0;
			virtual void setClearColor(Core::Maths::vec4<float32> color) = 0;
	};
}

#endif