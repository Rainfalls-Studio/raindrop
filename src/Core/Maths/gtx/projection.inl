/// @ref gtx_projection

namespace Raindrop::Core::Math
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType proj(genType const& x, genType const& Normal)
	{
		return Raindrop::Core::Math::dot(x, Normal) / Raindrop::Core::Math::dot(Normal, Normal) * Normal;
	}
}//namespace Raindrop::Core::Math
