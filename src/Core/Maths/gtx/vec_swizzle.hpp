/// @ref gtx_vec_swizzle
/// @file glm/gtx/vec_swizzle.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_vec_swizzle GLM_GTX_vec_swizzle
/// @ingroup gtx
///
/// Include <Core/Maths/gtx/vec_swizzle.hpp> to use the features of this extension.
///
/// Functions to perform swizzle operation.

#pragma once

#include "../glm.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	ifndef GLM_ENABLE_EXPERIMENTAL
#		pragma message("GLM: GLM_GTX_vec_swizzle is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it.")
#	else
#		pragma message("GLM: GLM_GTX_vec_swizzle extension included")
#	endif
#endif

namespace Raindrop::Core::Math {
	// xx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xx(const Raindrop::Core::Math::vec<1, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.x);
	}

	// xy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.y);
	}

	// xz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.z);
	}

	// xw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> xw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.x, v.w);
	}

	// yx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.x);
	}

	// yy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.y);
	}

	// yz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.z);
	}

	// yw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> yw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.y, v.w);
	}

	// zx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.x);
	}

	// zy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.y);
	}

	// zz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.z);
	}

	// zw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> zw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.z, v.w);
	}

	// wx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> wx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.w, v.x);
	}

	// wy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> wy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.w, v.y);
	}

	// wz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> wz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.w, v.z);
	}

	// ww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<2, T, Q> ww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<2, T, Q>(v.w, v.w);
	}

	// xxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxx(const Raindrop::Core::Math::vec<1, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.x);
	}

	// xxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.y);
	}

	// xxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.z);
	}

	// xxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.x, v.w);
	}

	// xyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.x);
	}

	// xyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.y);
	}

	// xyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.z);
	}

	// xyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.y, v.w);
	}

	// xzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.x);
	}

	// xzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.y);
	}

	// xzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.z);
	}

	// xzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.z, v.w);
	}

	// xwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.w, v.x);
	}

	// xwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.w, v.y);
	}

	// xwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.w, v.z);
	}

	// xww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> xww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.x, v.w, v.w);
	}

	// yxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.x);
	}

	// yxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.y);
	}

	// yxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.z);
	}

	// yxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.x, v.w);
	}

	// yyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.x);
	}

	// yyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.y);
	}

	// yyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.z);
	}

	// yyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.y, v.w);
	}

	// yzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.x);
	}

	// yzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.y);
	}

	// yzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.z);
	}

	// yzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.z, v.w);
	}

	// ywx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> ywx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.w, v.x);
	}

	// ywy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> ywy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.w, v.y);
	}

	// ywz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> ywz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.w, v.z);
	}

	// yww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> yww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.y, v.w, v.w);
	}

	// zxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.x);
	}

	// zxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.y);
	}

	// zxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.z);
	}

	// zxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.x, v.w);
	}

	// zyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.x);
	}

	// zyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.y);
	}

	// zyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.z);
	}

	// zyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.y, v.w);
	}

	// zzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.x);
	}

	// zzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.y);
	}

	// zzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.z);
	}

	// zzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.z, v.w);
	}

	// zwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.w, v.x);
	}

	// zwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.w, v.y);
	}

	// zwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.w, v.z);
	}

	// zww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> zww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.z, v.w, v.w);
	}

	// wxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.x, v.x);
	}

	// wxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.x, v.y);
	}

	// wxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.x, v.z);
	}

	// wxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.x, v.w);
	}

	// wyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.y, v.x);
	}

	// wyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.y, v.y);
	}

	// wyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.y, v.z);
	}

	// wyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.y, v.w);
	}

	// wzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.z, v.x);
	}

	// wzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.z, v.y);
	}

	// wzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.z, v.z);
	}

	// wzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.z, v.w);
	}

	// wwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.w, v.x);
	}

	// wwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.w, v.y);
	}

	// wwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> wwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.w, v.z);
	}

	// www
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<3, T, Q> www(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<3, T, Q>(v.w, v.w, v.w);
	}

	// xxxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxx(const Raindrop::Core::Math::vec<1, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	// xxxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	// xxxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.z);
	}

	// xxxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.x, v.w);
	}

	// xxyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	// xxyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	// xxyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.z);
	}

	// xxyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.y, v.w);
	}

	// xxzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.x);
	}

	// xxzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.y);
	}

	// xxzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.z);
	}

	// xxzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.z, v.w);
	}

	// xxwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.w, v.x);
	}

	// xxwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.w, v.y);
	}

	// xxwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.w, v.z);
	}

	// xxww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xxww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.x, v.w, v.w);
	}

	// xyxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	// xyxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	// xyxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.z);
	}

	// xyxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.x, v.w);
	}

	// xyyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	// xyyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	// xyyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.z);
	}

	// xyyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.y, v.w);
	}

	// xyzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.x);
	}

	// xyzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.y);
	}

	// xyzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.z);
	}

	// xyzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.z, v.w);
	}

	// xywx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xywx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.w, v.x);
	}

	// xywy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xywy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.w, v.y);
	}

	// xywz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xywz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.w, v.z);
	}

	// xyww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xyww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.y, v.w, v.w);
	}

	// xzxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.x);
	}

	// xzxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.y);
	}

	// xzxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.z);
	}

	// xzxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.x, v.w);
	}

	// xzyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.x);
	}

	// xzyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.y);
	}

	// xzyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.z);
	}

	// xzyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.y, v.w);
	}

	// xzzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.x);
	}

	// xzzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.y);
	}

	// xzzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.z);
	}

	// xzzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.z, v.w);
	}

	// xzwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.w, v.x);
	}

	// xzwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.w, v.y);
	}

	// xzwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.w, v.z);
	}

	// xzww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xzww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.z, v.w, v.w);
	}

	// xwxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.x, v.x);
	}

	// xwxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.x, v.y);
	}

	// xwxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.x, v.z);
	}

	// xwxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.x, v.w);
	}

	// xwyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.y, v.x);
	}

	// xwyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.y, v.y);
	}

	// xwyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.y, v.z);
	}

	// xwyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.y, v.w);
	}

	// xwzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.z, v.x);
	}

	// xwzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.z, v.y);
	}

	// xwzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.z, v.z);
	}

	// xwzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.z, v.w);
	}

	// xwwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.w, v.x);
	}

	// xwwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.w, v.y);
	}

	// xwwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.w, v.z);
	}

	// xwww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> xwww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.x, v.w, v.w, v.w);
	}

	// yxxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	// yxxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	// yxxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.z);
	}

	// yxxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.x, v.w);
	}

	// yxyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	// yxyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	// yxyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.z);
	}

	// yxyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.y, v.w);
	}

	// yxzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.x);
	}

	// yxzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.y);
	}

	// yxzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.z);
	}

	// yxzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.z, v.w);
	}

	// yxwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.w, v.x);
	}

	// yxwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.w, v.y);
	}

	// yxwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.w, v.z);
	}

	// yxww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yxww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.x, v.w, v.w);
	}

	// yyxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	// yyxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	// yyxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.z);
	}

	// yyxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.x, v.w);
	}

	// yyyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyx(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	// yyyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyy(const Raindrop::Core::Math::vec<2, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	// yyyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.z);
	}

	// yyyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.y, v.w);
	}

	// yyzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.x);
	}

	// yyzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.y);
	}

	// yyzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.z);
	}

	// yyzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.z, v.w);
	}

	// yywx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yywx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.w, v.x);
	}

	// yywy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yywy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.w, v.y);
	}

	// yywz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yywz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.w, v.z);
	}

	// yyww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yyww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.y, v.w, v.w);
	}

	// yzxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.x);
	}

	// yzxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.y);
	}

	// yzxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.z);
	}

	// yzxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.x, v.w);
	}

	// yzyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.x);
	}

	// yzyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.y);
	}

	// yzyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.z);
	}

	// yzyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.y, v.w);
	}

	// yzzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.x);
	}

	// yzzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.y);
	}

	// yzzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.z);
	}

	// yzzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.z, v.w);
	}

	// yzwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.w, v.x);
	}

	// yzwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.w, v.y);
	}

	// yzwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.w, v.z);
	}

	// yzww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> yzww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.z, v.w, v.w);
	}

	// ywxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.x, v.x);
	}

	// ywxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.x, v.y);
	}

	// ywxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.x, v.z);
	}

	// ywxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.x, v.w);
	}

	// ywyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.y, v.x);
	}

	// ywyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.y, v.y);
	}

	// ywyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.y, v.z);
	}

	// ywyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.y, v.w);
	}

	// ywzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.z, v.x);
	}

	// ywzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.z, v.y);
	}

	// ywzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.z, v.z);
	}

	// ywzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.z, v.w);
	}

	// ywwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.w, v.x);
	}

	// ywwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.w, v.y);
	}

	// ywwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.w, v.z);
	}

	// ywww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> ywww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.y, v.w, v.w, v.w);
	}

	// zxxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.x);
	}

	// zxxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.y);
	}

	// zxxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.z);
	}

	// zxxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.x, v.w);
	}

	// zxyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.x);
	}

	// zxyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.y);
	}

	// zxyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.z);
	}

	// zxyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.y, v.w);
	}

	// zxzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.x);
	}

	// zxzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.y);
	}

	// zxzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.z);
	}

	// zxzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.z, v.w);
	}

	// zxwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.w, v.x);
	}

	// zxwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.w, v.y);
	}

	// zxwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.w, v.z);
	}

	// zxww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zxww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.x, v.w, v.w);
	}

	// zyxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.x);
	}

	// zyxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.y);
	}

	// zyxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.z);
	}

	// zyxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.x, v.w);
	}

	// zyyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.x);
	}

	// zyyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.y);
	}

	// zyyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.z);
	}

	// zyyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.y, v.w);
	}

	// zyzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.x);
	}

	// zyzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.y);
	}

	// zyzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.z);
	}

	// zyzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.z, v.w);
	}

	// zywx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zywx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.w, v.x);
	}

	// zywy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zywy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.w, v.y);
	}

	// zywz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zywz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.w, v.z);
	}

	// zyww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zyww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.y, v.w, v.w);
	}

	// zzxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.x);
	}

	// zzxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.y);
	}

	// zzxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.z);
	}

	// zzxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.x, v.w);
	}

	// zzyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.x);
	}

	// zzyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.y);
	}

	// zzyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.z);
	}

	// zzyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.y, v.w);
	}

	// zzzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzx(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.x);
	}

	// zzzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzy(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.y);
	}

	// zzzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzz(const Raindrop::Core::Math::vec<3, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.z);
	}

	// zzzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.z, v.w);
	}

	// zzwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.w, v.x);
	}

	// zzwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.w, v.y);
	}

	// zzwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.w, v.z);
	}

	// zzww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zzww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.z, v.w, v.w);
	}

	// zwxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.x, v.x);
	}

	// zwxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.x, v.y);
	}

	// zwxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.x, v.z);
	}

	// zwxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.x, v.w);
	}

	// zwyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.y, v.x);
	}

	// zwyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.y, v.y);
	}

	// zwyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.y, v.z);
	}

	// zwyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.y, v.w);
	}

	// zwzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.z, v.x);
	}

	// zwzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.z, v.y);
	}

	// zwzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.z, v.z);
	}

	// zwzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.z, v.w);
	}

	// zwwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.w, v.x);
	}

	// zwwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.w, v.y);
	}

	// zwwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.w, v.z);
	}

	// zwww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> zwww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.z, v.w, v.w, v.w);
	}

	// wxxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.x, v.x);
	}

	// wxxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.x, v.y);
	}

	// wxxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.x, v.z);
	}

	// wxxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.x, v.w);
	}

	// wxyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.y, v.x);
	}

	// wxyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.y, v.y);
	}

	// wxyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.y, v.z);
	}

	// wxyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.y, v.w);
	}

	// wxzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.z, v.x);
	}

	// wxzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.z, v.y);
	}

	// wxzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.z, v.z);
	}

	// wxzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.z, v.w);
	}

	// wxwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.w, v.x);
	}

	// wxwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.w, v.y);
	}

	// wxwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.w, v.z);
	}

	// wxww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wxww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.x, v.w, v.w);
	}

	// wyxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.x, v.x);
	}

	// wyxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.x, v.y);
	}

	// wyxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.x, v.z);
	}

	// wyxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.x, v.w);
	}

	// wyyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.y, v.x);
	}

	// wyyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.y, v.y);
	}

	// wyyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.y, v.z);
	}

	// wyyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.y, v.w);
	}

	// wyzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.z, v.x);
	}

	// wyzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.z, v.y);
	}

	// wyzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.z, v.z);
	}

	// wyzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.z, v.w);
	}

	// wywx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wywx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.w, v.x);
	}

	// wywy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wywy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.w, v.y);
	}

	// wywz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wywz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.w, v.z);
	}

	// wyww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wyww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.y, v.w, v.w);
	}

	// wzxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.x, v.x);
	}

	// wzxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.x, v.y);
	}

	// wzxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.x, v.z);
	}

	// wzxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.x, v.w);
	}

	// wzyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.y, v.x);
	}

	// wzyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.y, v.y);
	}

	// wzyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.y, v.z);
	}

	// wzyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.y, v.w);
	}

	// wzzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.z, v.x);
	}

	// wzzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.z, v.y);
	}

	// wzzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.z, v.z);
	}

	// wzzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.z, v.w);
	}

	// wzwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.w, v.x);
	}

	// wzwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.w, v.y);
	}

	// wzwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.w, v.z);
	}

	// wzww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wzww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.z, v.w, v.w);
	}

	// wwxx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwxx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.x, v.x);
	}

	// wwxy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwxy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.x, v.y);
	}

	// wwxz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwxz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.x, v.z);
	}

	// wwxw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwxw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.x, v.w);
	}

	// wwyx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwyx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.y, v.x);
	}

	// wwyy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwyy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.y, v.y);
	}

	// wwyz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwyz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.y, v.z);
	}

	// wwyw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwyw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.y, v.w);
	}

	// wwzx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwzx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.z, v.x);
	}

	// wwzy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwzy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.z, v.y);
	}

	// wwzz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwzz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.z, v.z);
	}

	// wwzw
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwzw(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.z, v.w);
	}

	// wwwx
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwwx(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.w, v.x);
	}

	// wwwy
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwwy(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.w, v.y);
	}

	// wwwz
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwwz(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.w, v.z);
	}

	// wwww
	template<typename T, qualifier Q>
	GLM_INLINE Raindrop::Core::Math::vec<4, T, Q> wwww(const Raindrop::Core::Math::vec<4, T, Q> &v) {
		return Raindrop::Core::Math::vec<4, T, Q>(v.w, v.w, v.w, v.w);
	}

}
