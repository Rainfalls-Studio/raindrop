#ifndef __RAINDROP_CORE_MATHS_MATHS_HPP__
#define __RAINDROP_CORE_MATHS_MATHS_HPP__

#include <glm/glm.hpp>
#include "Core/common.hpp"
#include "Core/core.hpp"

namespace Raindrop::Core::Maths{
	template<uint32 L, typename T>
	using vec = glm::vec<L, T>;

	template<uint32 W, uint32 L, typename T>
	using mat = glm::mat<W, L, T>;

	template<typename T>
	using vec1 = vec<1, T>;

	template<typename T>
	using vec2 = vec<2, T>;

	template<typename T>
	using vec3 = vec<3, T>;

	template<typename T>
	using vec4 = vec<4, T>;

	template<typename T>
	using mat2x2 = mat<2, 2, T>;
	
	template<typename T>
	using mat2x3 = mat<2, 3, T>;
	
	template<typename T>
	using mat2x4 = mat<2, 4, T>;
	
	template<typename T>
	using mat3x2 = mat<3, 2, T>;
	
	template<typename T>
	using mat3x3 = mat<3, 3, T>;
	
	template<typename T>
	using mat3x4 = mat<3, 4, T>;

	template<typename T>
	using mat4x2 = mat<4, 2, T>;
	
	template<typename T>
	using mat4x3 = mat<4, 3, T>;
	
	template<typename T>
	using mat4x4 = mat<4, 4, T>;

	template<typename T>
	inline T min(const T& x, const T& y){
		return glm::min(x, y);
	}

	template<uint32 L, typename T>
	inline vec<L, T> max(const vec<L, T>& x, const vec<L, T>& y){
		return glm::max(x, y);
	}

	template<uint32 L, typename T>
	inline vec<L, T> radians(const vec<L, T>& degrees){
		return glm::radians(degrees);
	}

	template<uint32 L, typename T>
	inline vec<L, T> degrees(const vec<L, T>& radians){
		return glm::degrees(radians);
	}

	template<uint32 L, typename T>
	inline vec<L, T> sin(const vec<L, T>& angle){
		return glm::sin(angle);
	}

	template<uint32 L, typename T>
	inline vec<L, T> cos(const vec<L, T>& angle){
		return glm::cos(angle);
	}

	template<uint32 L, typename T>
	inline vec<L, T> tan(const vec<L, T>& angle){
		return glm::tan(angle);
	}

	template<uint32 L, typename T>
	inline vec<L, T> asin(const vec<L, T>& x){
		return glm::asin(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> acos(const vec<L, T>& x){
		return glm::acos(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> atan(const vec<L, T>& x){
		return glm::atan(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> pow(const vec<L, T>& x, const vec<L, T>& exponent){
		return glm::pow(x, exponent);
	}

	template<uint32 L, typename T>
	inline vec<L, T> exp(const vec<L, T>& x){
		return glm::exp(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> log(const vec<L, T>& x){
		return glm::log(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> exp2(const vec<L, T>& x){
		return glm::exp2(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> log2(const vec<L, T>& x){
		return glm::log2(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> sqrt(const vec<L, T>& x){
		return glm::sqrt(x);
	}

	template<uint32 L, typename T>
	inline vec<L, T> inversesqrt(const vec<L, T>& x){
		return glm::inversesqrt(x);
	}

	template<uint32 L, typename T>
	inline T lenght(const vec<L, T>& x){
		return glm::length(x);
	}

	template<uint32 L, typename T>
	inline T distance(const vec<L, T>& p0, const vec<L, T>& p1){
		return glm::distance(p0, p1);
	}

	template<uint32 L, typename T>
	inline T dot(const vec<L, T>& x, const vec<L, T>& y){
		return glm::dot(x, y);
	}

	template<typename T>
	inline bool isPowerOfTwo(const T& x){
		return !(x == 0) && !(x & (x - 1));
	}

	template<uint32 L, typename T>
	inline vec<L, T> cross(const vec<L, T>& x, const vec<L, T>& y){
		return glm::cross(x, y);
	}

	template<uint32 L, typename T>
	inline vec<L, T> normalize(vec<L, T> const &x){
		return glm::normalize(x);
	}

	template<uint32 W, uint32 L, typename T>
	inline mat<W, L, T> matrixCompMult(const mat<W, L, T>& x, const mat<W, L, T>& y){
		return glm::matrixCompMult(x, y);
	}

	template<uint32 W, uint32 L, typename T>
	inline mat<W, L, T> transpose(const mat<W, L, T>& x){
		return glm::transpose(x);
	}

	template<uint32 W, uint32 L, typename T>
	inline T determinant(const mat<W, L, T>& x){
		return glm::determinant(x);
	}
	
	template<uint32 W, uint32 L, typename T>
	inline mat<W, L, T> inverse(const mat<W, L, T>& x){
		return glm::inverse(x);
	}

}

#endif