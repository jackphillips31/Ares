#include <arespch.h>

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/Utility/Type.h"

namespace Ares {

	namespace Utility {

		namespace Type {

			const std::unordered_map<std::type_index, uint32_t> g_TypeMap{
				{typeid(::Ares::VertexShader), Type::VertexShader},
				{typeid(::Ares::FragmentShader), Type::FragmentShader},
				{typeid(::Ares::ShaderProgram), Type::ShaderProgram},
				{typeid(::Ares::Texture), Type::Texture}
			};

			const uint32_t GetTypeMapId(const std::type_index& type)
			{
				auto it = g_TypeMap.find(type);
				if (it != g_TypeMap.end())
					return it->second;
				else
					return 0;
			}

			std::string ExtractClassName(const std::string& typeName)
			{
				// Find the last occurrence of "::"
				size_t pos = typeName.rfind("::");
				if (pos != std::string::npos)
				{
					// Return everything after "::"
					return typeName.substr(pos + 2);
				}

				// Otherwise, check if the type contains a keyword like "class" or "struct"
				if (typeName.find("class ") == 0)
				{
					// Remove "class "
					return typeName.substr(6);
				}
				else if (typeName.find("struct ") == 0)
				{
					// Remove "struct "
					return typeName.substr(7);
				}

				// If no namespace or keyword is found, return the typeName as-is
				return typeName;
			}

		}

	}

}