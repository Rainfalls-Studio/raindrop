/// @ref core
/// @file glm/ext/vector_float4.hpp

#pragma once
#include "../detail/type_vec4.hpp"

namespace Raindrop::Core::Math
{
	/// @addtogroup core_vector
	/// @{

	/// 4 components vector of single-precision floating-point numbers.
	///
	/// @see <a href="http://www.opengl.org/registry/doc/GLSLangSpec.4.20.8.pdf">GLSL 4.20.8 specification, section 4.1.5 Vectors</a>
	typedef vec<4, float, defaultp>		vec4;

	/// @}
}//namespace Raindrop::Core::Math