/// @ref gtx_texture

namespace Raindrop::Core::Math
{
	template <length_t L, typename T, qualifier Q>
	inline T levels(vec<L, T, Q> const& Extent)
	{
		return Raindrop::Core::Math::log2(compMax(Extent)) + static_cast<T>(1);
	}

	template <typename T>
	inline T levels(T Extent)
	{
		return vec<1, T, defaultp>(Extent).x;
	}
}//namespace Raindrop::Core::Math

