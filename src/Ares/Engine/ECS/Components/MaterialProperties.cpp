#include <arespch.h>
#include "Engine/ECS/Components/MaterialProperties.h"

namespace Ares::ECS::Components {

	MaterialProperties::BasicPropsProxy::BasicPropsProxy(uint8_t* data)
		: m_Data(data),
		Color(*reinterpret_cast<glm::vec3*>(data)),
		Alpha(*reinterpret_cast<float*>(data + sizeof(glm::vec3)))
	{
		Color = { 1.0f, 1.0f, 1.0f };
		Alpha = 1.0f;
	}

	MaterialProperties::BasicPropsProxy& MaterialProperties::BasicPropsProxy::operator=(const MaterialProperties::BasicPropsProxy& other)
	{
		Color = other.Color;
		Alpha = other.Alpha;
		return *this;
	}

	MaterialProperties::SurfacePropsProxy::SurfacePropsProxy(uint8_t* data)
		: m_Data(data),
		Roughness(*reinterpret_cast<float*>(data)),
		Metallic(*reinterpret_cast<float*>(data + sizeof(float))),
		Reflectivity(*reinterpret_cast<float*>(data + 2 * sizeof(float))),
		EmissiveColor(*reinterpret_cast<glm::vec3*>(data + 3 * sizeof(float))),
		EmissiveIntensity(*reinterpret_cast<float*>(data + 3 * sizeof(float) + sizeof(glm::vec3)))
	{
		Roughness = 0.2f;
		Metallic = 0.1f;
		Reflectivity = 0.1f;
		EmissiveColor = { 1.0f, 1.0f, 1.0f };
		EmissiveIntensity = 0.0f;
	}

	MaterialProperties::SurfacePropsProxy& MaterialProperties::SurfacePropsProxy::operator=(const MaterialProperties::SurfacePropsProxy& other)
	{
		Roughness = other.Roughness;
		Metallic = other.Metallic;
		Reflectivity = other.Reflectivity;
		EmissiveColor = other.EmissiveColor;
		EmissiveIntensity = other.EmissiveIntensity;
		return *this;
	}

	/*
	MaterialProperties::RenderPropsProxy::RenderPropsProxy(uint8_t* data)
		: m_Data(data),
		ReceiveShadows(data),
		CastShadows(data + sizeof(int32_t)),
		Wireframe(data + 2 * sizeof(int32_t))
	{
		ReceiveShadows = true;
		CastShadows = true;
		Wireframe = false;
	}

	MaterialProperties::RenderPropsProxy& MaterialProperties::RenderPropsProxy::operator=(const MaterialProperties::RenderPropsProxy& other)
	{
		ReceiveShadows = other.ReceiveShadows;
		CastShadows = other.CastShadows;
		Wireframe = other.Wireframe;
		return *this;
	}
	*/

	MaterialProperties::MaterialProperties()
		: m_Buffer(44),
		Basic(m_Buffer.data()),
		Surface(m_Buffer.data() + 16)
		//Render(m_Buffer.data() + 16 + 28)
	{
	}

	MaterialProperties::MaterialProperties(const MaterialProperties& other)
		: m_Buffer(other.m_Buffer),
		Basic(m_Buffer.data()),
		Surface(m_Buffer.data() + 16)
		//Render(m_Buffer.data() + 16 + 28)
	{
		Basic.Color = other.Basic.Color;
		Basic.Alpha = other.Basic.Alpha;
		Surface.Roughness = other.Surface.Roughness;
		Surface.Metallic = other.Surface.Metallic;
		Surface.Reflectivity = other.Surface.Reflectivity;
		Surface.EmissiveColor = other.Surface.EmissiveColor;
		Surface.EmissiveIntensity = other.Surface.EmissiveIntensity;
		//Render.ReceiveShadows = other.Render.ReceiveShadows;
		//Render.CastShadows = other.Render.CastShadows;
		//Render.Wireframe = other.Render.Wireframe;
	}

	MaterialProperties& MaterialProperties::operator=(const MaterialProperties& other)
	{
		if (this != &other)
		{
			m_Buffer = other.m_Buffer;
		}
		return *this;
	}

	const void* MaterialProperties::GetBuffer() const
	{
		return m_Buffer.data();
	}

	size_t MaterialProperties::GetSize() const
	{
		return m_Buffer.size();
	}

}