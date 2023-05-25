#ifndef __RAINDROP_CORE_TOOLS_HPP__
#define __RAINDROP_CORE_TOOLS_HPP__

#include <Raindrop/common.hpp>

namespace Raindrop::Core{
	static bool iequals(const std::string& a, const std::string& b){
		return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {return tolower(a) == tolower(b);});
	}

	static bool iequals(const std::filesystem::path& a, const std::filesystem::path& b){
		auto stringA = a.wstring();
		auto stringB = b.wstring();

		std::transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
		std::transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

		return (stringA == stringB);
	}
}

#endif