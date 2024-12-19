#pragma once

namespace Ares {

	namespace Utility {

		namespace Type {

			enum Types : uint32_t
			{
				None = 0,
				VertexShader,
				FragmentShader,
				ShaderProgram,
				Texture2D,
			};

			const uint32_t GetTypeMapId(const std::type_index& type);

			std::string ExtractClassName(const std::string& typeName);

		}

	}

}