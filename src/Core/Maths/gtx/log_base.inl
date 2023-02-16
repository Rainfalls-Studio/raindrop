/// @ref gtx_log_base

namespace Raindrop::Core::Math
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType log(genType const& x, genType const& base)
	{
		return Raindrop::Core::Math::log(x) / Raindrop::Core::Math::log(base);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> log(vec<L, T, Q> const& x, vec<L, T, Q> const& base)
	{
		return Raindrop::Core::Math::log(x) / Raindrop::Core::Math::log(base);
	}
}//namespace Raindrop::Core::Math
