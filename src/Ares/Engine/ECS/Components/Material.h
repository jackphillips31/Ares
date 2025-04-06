#pragma once
#include <variant>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Engine/Containers/HashMap.h"
#include "Engine/Containers/String.h"
#include "Engine/Containers/Variant.h"
#include "Engine/ECS/Components/MaterialProperties.h"
#include "Engine/ECS/Core/Component.h"

namespace Ares {

	class Asset;
	class ShaderProgram;
	
	namespace ECS::Components {

		class Material : public Component
		{
		public:
			// Constructor
			Material();
			Material(const Ref<Asset>& shaderAsset);

			// Copy constructors
			Material(const Material& other);
			Material& operator=(const Material& other);

			// Getters
			StringView GetShaderName() const;
			size_t GetShaderSize() const;
			MaterialProperties GetProperties() const;
			ShaderProgram* GetShader() const;

			// Material properties
			void SetShader(const Ref<Asset>& asset);
			template <typename PropertyType>
			void SetUniformProperty(const String& name, const PropertyType& value);
			void SetTexture(const String& name, const Ref<Asset>& texture);
			void SetProperties(const MaterialProperties& props);

			// Asset properties
			bool IsLoaded() const;
			bool IsValid() const;
			void PreCache() const;

			void Bind() const;

		private:
			// Mutex for thread-safety
			mutable std::shared_mutex m_Mutex;

			// Material assets
			Ref<Asset> m_ShaderAsset;
			HashMap<String, Ref<Asset>> m_TextureAssets;

			// Uniform properties
			HashMap<String, Variant<int32_t, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4>> m_Properties;

			// Material properties
			MaterialProperties m_MaterialProperties;

			// Hash
			friend struct std::hash<Material>;
			friend struct eastl::hash<Material>;
		};

	}

}

namespace eastl {

	template<>
	struct hash<Ares::ECS::Components::Material>
	{
		size_t operator()(const Ares::ECS::Components::Material& material) const;
	};

}