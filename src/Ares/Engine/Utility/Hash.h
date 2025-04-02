#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <typeindex>
#include <EASTL/string.h>

namespace Ares {

	enum class EventType : uint16_t;

	template <typename T>
	void CombineHash(size_t& seed, const T& value)
	{
		seed ^= eastl::hash<T>{}(value)+0xB000B1E5 + (seed << 6) + (seed >> 2);
	}

}

// EASTL Hashes
namespace eastl {

	// Standard Library Hashes
	template<>
	struct hash<std::type_index>
	{
		size_t operator()(const std::type_index& typeIndex) const
		{
			return typeIndex.hash_code();
		}
	};
	template<>
	struct hash<std::string>
	{
		size_t operator()(const std::string& string) const
		{
			return eastl::hash<const char*>()(string.c_str());
		}
	};

	// GLM Hashes
	template<>
	struct hash<glm::vec2>
	{
		size_t operator()(const glm::vec2& vec) const
		{
			size_t hash = static_cast<size_t>(vec.x);
			Ares::CombineHash<float>(hash, vec.y);
			return hash;
		}
	};
	template<>
	struct hash<glm::vec3>
	{
		size_t operator()(const glm::vec3& vec) const
		{
			size_t hash = static_cast<size_t>(vec.x);
			Ares::CombineHash<float>(hash, vec.y);
			Ares::CombineHash<float>(hash, vec.z);
			return hash;
		}
	};
	template<>
	struct hash<glm::vec4>
	{
		size_t operator()(const glm::vec4& vec) const
		{
			size_t hash = static_cast<size_t>(vec.x);
			Ares::CombineHash<float>(hash, vec.y);
			Ares::CombineHash<float>(hash, vec.z);
			Ares::CombineHash<float>(hash, vec.w);
			return hash;
		}
	};

}