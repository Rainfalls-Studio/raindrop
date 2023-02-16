/// @ref gtx_hash
///
/// @see core (dependence)
///
/// @defgroup gtx_hash GLM_GTX_hash
/// @ingroup gtx
///
/// @brief Add std::hash support for glm types
///
/// <Core/Maths/gtx/hash.inl> need to be included to use the features of this extension.

namespace Raindrop::Core::Math {
namespace detail
{
	GLM_INLINE void hash_combine(size_t &seed, size_t hash)
	{
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
	}
}}

namespace std
{
	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::vec<1, T, Q>>::operator()(Raindrop::Core::Math::vec<1, T, Q> const& v) const
	{
		hash<T> hasher;
		return hasher(v.x);
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::vec<2, T, Q>>::operator()(Raindrop::Core::Math::vec<2, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.x));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.y));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::vec<3, T, Q>>::operator()(Raindrop::Core::Math::vec<3, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.x));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.y));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.z));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::vec<4, T, Q>>::operator()(Raindrop::Core::Math::vec<4, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.x));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.y));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.z));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(v.w));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::qua<T, Q>>::operator()(Raindrop::Core::Math::qua<T,Q> const& q) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.x));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.y));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.z));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.w));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::tdualquat<T, Q>>::operator()(Raindrop::Core::Math::tdualquat<T, Q> const& q) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::qua<T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.real));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(q.dual));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<2, 2, T, Q>>::operator()(Raindrop::Core::Math::mat<2, 2, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<2, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<2, 3, T, Q>>::operator()(Raindrop::Core::Math::mat<2, 3, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<3, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<2, 4, T, Q>>::operator()(Raindrop::Core::Math::mat<2, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<4, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<3, 2, T, Q>>::operator()(Raindrop::Core::Math::mat<3, 2, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<2, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<3, 3, T, Q>>::operator()(Raindrop::Core::Math::mat<3, 3, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<3, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<3, 4, T, Q>>::operator()(Raindrop::Core::Math::mat<3, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<4, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<4, 2, T,Q>>::operator()(Raindrop::Core::Math::mat<4, 2, T,Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<2, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<4, 3, T,Q>>::operator()(Raindrop::Core::Math::mat<4, 3, T,Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<3, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}

	template<typename T, Raindrop::Core::Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Raindrop::Core::Math::mat<4, 4, T,Q>>::operator()(Raindrop::Core::Math::mat<4, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Raindrop::Core::Math::vec<4, T, Q>> hasher;
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[0]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[1]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[2]));
		Raindrop::Core::Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}
}
