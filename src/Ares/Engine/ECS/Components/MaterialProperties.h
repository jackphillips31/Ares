#pragma once
#include <glm/vec3.hpp>

namespace Ares::ECS::Components {

	class MaterialProperties
	{
	private:
		std::vector<uint8_t> m_Buffer;

		class BasicPropsProxy
		{
		public:
			glm::vec3& Color;
			float& Alpha;

			BasicPropsProxy(uint8_t* data);

			BasicPropsProxy(const BasicPropsProxy& other) = delete;
			BasicPropsProxy& operator=(const BasicPropsProxy& other);

		private:
			uint8_t* m_Data;
		};

		class SurfacePropsProxy
		{
		public:
			float& Roughness;
			float& Metallic;
			float& Reflectivity;
			glm::vec3& EmissiveColor;
			float& EmissiveIntensity;

			SurfacePropsProxy(uint8_t* data);

			SurfacePropsProxy(const SurfacePropsProxy& other) = delete;
			SurfacePropsProxy& operator=(const SurfacePropsProxy& other);

		private:
			uint8_t* m_Data;
		};

		/*
		class RenderPropsProxy
		{
		public:
			struct BoolProp
			{
				int32_t& value;

				BoolProp(uint8_t* data) : value(*reinterpret_cast<int32_t*>(data)) {}

				operator bool() const { return value != 0; }
				BoolProp& operator=(bool setVal) { value = setVal ? 1 : 0; return *this; }
				BoolProp& operator=(const BoolProp& other) { value = other.value; return *this; }
			};
			BoolProp ReceiveShadows;
			BoolProp CastShadows;
			BoolProp Wireframe;

			RenderPropsProxy(uint8_t* data);

			RenderPropsProxy(const RenderPropsProxy& other) = delete;
			RenderPropsProxy& operator=(const RenderPropsProxy& other);

		private:
			uint8_t* m_Data;
		};
		*/

	public:
		BasicPropsProxy Basic;
		SurfacePropsProxy Surface;
		//RenderPropsProxy Render;

		MaterialProperties();

		const void* GetBuffer() const;
		size_t GetSize() const;

		MaterialProperties(const MaterialProperties& other);
		MaterialProperties& operator=(const MaterialProperties& other);
	};

}