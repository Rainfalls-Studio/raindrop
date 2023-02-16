/// @ref gtx_extended_min_max

namespace Raindrop::Core::Math
{
	template<typename T>
	GLM_FUNC_QUALIFIER T min(
		T const& x,
		T const& y,
		T const& z)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z
	)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z
	)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), z);
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T min
	(
		T const& x,
		T const& y,
		T const& z,
		T const& w
	)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), Raindrop::Core::Math::min(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z,
		typename C<T>::T const& w
	)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), Raindrop::Core::Math::min(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> min
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z,
		C<T> const& w
	)
	{
		return Raindrop::Core::Math::min(Raindrop::Core::Math::min(x, y), Raindrop::Core::Math::min(z, w));
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T max(
		T const& x,
		T const& y,
		T const& z)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z
	)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), z);
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z
	)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), z);
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T max
	(
		T const& x,
		T const& y,
		T const& z,
		T const& w
	)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), Raindrop::Core::Math::max(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		typename C<T>::T const& y,
		typename C<T>::T const& z,
		typename C<T>::T const& w
	)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), Raindrop::Core::Math::max(z, w));
	}

	template<typename T, template<typename> class C>
	GLM_FUNC_QUALIFIER C<T> max
	(
		C<T> const& x,
		C<T> const& y,
		C<T> const& z,
		C<T> const& w
	)
	{
		return Raindrop::Core::Math::max(Raindrop::Core::Math::max(x, y), Raindrop::Core::Math::max(z, w));
	}
}//namespace Raindrop::Core::Math
