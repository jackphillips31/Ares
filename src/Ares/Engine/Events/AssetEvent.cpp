#include <arespch.h>
#include "Engine/Events/AssetEvent.h"

#include "Engine/Data/Asset.h"
#include "Engine/Renderer/Assets/MeshData.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"

namespace Ares {

	template <typename AssetType>
	AssetType* AssetBaseEvent::GetRawAsset() const
	{
		if (m_AssetState == AssetState::Loaded)
			return m_Asset->GetAsset<AssetType>();
		else
			return nullptr;
	}

	template VertexShader* AssetBaseEvent::GetRawAsset<VertexShader>() const;
	template FragmentShader* AssetBaseEvent::GetRawAsset<FragmentShader>() const;
	template ShaderProgram* AssetBaseEvent::GetRawAsset<ShaderProgram>() const;
	template MeshData* AssetBaseEvent::GetRawAsset<MeshData>() const;
	template Texture* AssetBaseEvent::GetRawAsset<Texture>() const;

	void AssetBaseEvent::Initialize(const Ref<Asset>& asset, const std::string& message)
	{
		m_Asset = asset;
		m_Message = message;
		m_AssetName = asset->GetName();
		m_AssetFilepath = asset->GetFilepath();
		m_AssetStateString = asset->GetStateString();
		m_AssetState = asset->GetState();
	}

	AssetStagedEvent::AssetStagedEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

	AssetLoadingEvent::AssetLoadingEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

	AssetLoadedEvent::AssetLoadedEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

	AssetFailedEvent::AssetFailedEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

	AssetUnloadedEvent::AssetUnloadedEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

	AssetUnstagedEvent::AssetUnstagedEvent(const Ref<Asset>& asset, const std::string& message)
	{
		Initialize(asset, message);
	}

}