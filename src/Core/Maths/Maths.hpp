#ifndef __RAINDROP_CORE_MATHS_MATHS_HPP__
#define __RAINDROP_CORE_MATHS_MATHS_HPP__

#include <glm.hpp>
#include "../common.hpp"
#include "../core.hpp"

namespace Raindrop::Core::Maths{
	template<uint32 lenght, typename T>
	using vec = glm::vec<lenght, T>;

	template<uint32 width, uint32 lenght, typename T>
	using mat = glm::mat<width, height, T>;

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

	template<uint32 lenght, typename T>
	inline vec<lenght, T> radians(const vec<lenght, T>& degrees){
		return glm::radians(degrees);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> degrees(const vec<lenght, T>& radians){
		return glm::degrees(radians);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> sin(const vec<lenght, T>& angle){
		return glm::sin(angle);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> cos(const vec<lenght, T>& angle){
		return glm::cos(angle);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> tan(const vec<lenght, T>& angle){
		return glm::tan(angle);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> asin(const vec<lenght, T>& x){
		return glm::asin(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> acos(const vec<lenght, T>& x){
		return glm::acos(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> atan(const vec<lenght, T>& x){
		return glm::atan(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> pow(const vec<lenght, T>& x, const vec<lenght, T>& exponent){
		return glm::pow(x, exponent);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> exp(const vec<lenght, T>& x){
		return glm::exp(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> log(const vec<lenght, T>& x){
		return glm::log(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> exp2(const vec<lenght, T>& x){
		return glm::exp2(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> log2(const vec<lenght, T>& x){
		return glm::log2(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> sqrt(const vec<lenght, T>& x){
		return glm::sqrt(x);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> inversesqrt(const vec<lenght, T>& x){
		return glm::inversesqrt(x);
	}

	template<uint32 lenght, typename T>
	inline T lenght(const vec<lenght, T>& x){
		return glm::lenght(x);
	}

	template<uint32 lenght, typename T>
	inline T distance(const vec<lenght, T>& p0, const vec<lenght, T>& p1){
		return glm::distance(p0, p1);
	}

	template<uint32 lenght, typename T>
	inline T dot(const vec<lenght, T>& x, const vec<lenght, T>& y){
		return glm::dot(x, y);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> cross(const vec<lenght, T>& x, const vec<lenght, T>& y){
		return glm::cross(x, y);
	}

	template<uint32 lenght, typename T>
	inline vec<lenght, T> normalize(vec<lenght, T> const &x){
		return glm::normalize(x);
	}

	template<uint32 width, uint32 lenght, typename T>
	inline mat<width, height, T> matrixCompMult(const mat<width, height, T>& x, const mat<width, height, T>& y){
		return glm::matrixCompMult(x, y);
	}

	template<uint32 width, uint32 lenght, typename T>
	inline mat<width, height, T> transpose(const mat<width, height, T>& x){
		return glm::transpose(x);
	}

	template<uint32 width, uint32 lenght, typename T>
	inline T determinant(const mat<width, height, T>& x){
		return glm::determinant(x);
	}
	
	template<uint32 width, uint32 lenght, typename T>
	inline mat<width, height, T> inverse(const mat<width, height, T>& x){
		return glm::inverse(x);
	}
}

#endif