/// @ref gtx_normalize_dot

namespace Raindrop::Core::Math
{
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T normalizeDot(vec<L, T, Q> const& x, vec<L, T, Q> const& y)
	{
		return Raindrop::Core::Math::dot(x, y) * Raindrop::Core::Math::inversesqrt(Raindrop::Core::Math::dot(x, x) * Raindrop::Core::Math::dot(y, y));
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T fastNormalizeDot(vec<L, T, Q> const& x, vec<L, T, Q> const& y)
	{
		return Raindrop::Core::Math::dot(x, y) * Raindrop::Core::Math::fastInverseSqrt(Raindrop::Core::Math::dot(x, x) * Raindrop::Core::Math::dot(y, y));
	}
}//namespace Raindrop::Core::Math
