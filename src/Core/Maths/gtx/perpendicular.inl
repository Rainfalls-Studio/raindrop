/// @ref gtx_perpendicular

namespace Raindrop::Core::Math
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType perp(genType const& x, genType const& Normal)
	{
		return x - proj(x, Normal);
	}
}//namespace Raindrop::Core::Math
