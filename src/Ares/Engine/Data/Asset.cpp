#include <arespch.h>
#include "Engine/Data/Asset.h"

#include "Engine/Data/AssetManager.h"
#include "Engine/Data/DataBuffer.h"
#include "Engine/Data/MemoryDataProvider.h"
#include "Engine/Utility/Hash.h"
#include "Engine/Utility/Type.h"

namespace Ares {

	Ref<Asset> Asset::Create(
		const std::type_index& type,
		const AssetState state,
		StringView filepath,
		const Vector<uint32_t>& dependencies,
		const MemoryDataKey dataKey,
		Systems::AssetManager* parentManager
	)
	{
		// CreateRef (std::make_shared) doesn't have access to private
		// constructors, so we wrap a raw pointer with a smart pointer
		// instead.
		//return Ref<Asset>(new Asset(type, state, filepath, dependencies, dataKey, parentManager));
		return CreateRef<Asset>(type, state, filepath, dependencies, dataKey, parentManager);
	}

	Asset::Asset(
		const std::type_index& type,
		const AssetState state,
		StringView filepath,
		const Vector<uint32_t>& dependencies,
		const MemoryDataKey dataKey,
		Systems::AssetManager* parentManager
	)
		: m_Name(""),
		m_Filepath(""),
		m_TypeName(Utility::ExtractClassName(type)),
		m_Type(type),
		m_Dependencies(dependencies),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(state),
		m_DataKey(dataKey),
		m_AssetManager(parentManager)
	{
	}

	Asset::Asset()
		: m_Name(""),
		m_Filepath(""),
		m_TypeName(Utility::ExtractClassName(typeid(void))),
		m_Type(typeid(void)),
		m_Dependencies({}),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(AssetState::None),
		m_DataKey(0)
	{
	}

	Asset::~Asset()
	{
		m_Asset.reset();
	}

	void Asset::Load(eastl::function<void(Ref<Asset>)> callback)
	{
		m_AssetManager->Load(m_AssetManager->GetAsset(m_AssetId), eastl::move(callback));
	}

	StringView Asset::GetStateString() const
	{
		std::shared_lock lock(m_Mutex);
		switch (m_State)
		{
		case AssetState::None: return "None";
		case AssetState::Staged: return "Staged";
		case AssetState::Loading: return "Loading";
		case AssetState::Loaded: return "Loaded";
		case AssetState::Failed: return "Failed";
		}

		return "Unknown Asset State!";
	}

	size_t Asset::GetDataSize() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_DataKey != 0)
		{
			const DataBuffer& data = MemoryDataProvider::GetData(m_DataKey);
			return data.GetSize();
		}
		return 0;
	}

	void Asset::SetName(StringView name)
	{
		std::unique_lock lock(m_Mutex);
		m_Name = name;
	}

	void Asset::SetState(const AssetState state)
	{
		std::unique_lock lock(m_Mutex);
		m_State = state;
	}

	void Asset::SetAssetId(const uint32_t id)
	{
		std::unique_lock lock(m_Mutex);
		m_AssetId = id;
	}

	void Asset::SetAsset(Scope<AssetBase>&& asset)
	{
		std::unique_lock lock(m_Mutex);
		m_Asset = std::move(asset);
	}

	void Asset::SetDataKey(const MemoryDataKey dataKey)
	{
		std::unique_lock lock(m_Mutex);
		m_DataKey = dataKey;
	}

	void Asset::Unload()
	{
		std::unique_lock lock(m_Mutex);
		if (!m_Filepath.empty())
			m_DataKey = 0;

		m_Asset.reset();
		m_State = AssetState::Staged;
	}

	void Asset::Unstage()
	{
		std::unique_lock lock(m_Mutex);
		m_Name = "";
		m_Filepath = "";
		m_Type = typeid(void);
		m_TypeName = Utility::ExtractClassName(m_Type);
		m_Dependencies.clear();
		m_AssetId = 0;
		m_Asset = nullptr;
		m_State = AssetState::None;
		m_DataKey = 0;
	}

}

namespace eastl {

	size_t hash<Ares::Asset>::operator()(const Ares::Asset& asset) const
	{
		size_t seed = asset.m_Type.hash_code();

		if (asset.m_Filepath != nullptr)
			Ares::CombineHash<const char*>(seed, asset.m_Filepath.data());
		else if (asset.m_DataKey)
			Ares::CombineHash<uint32_t>(seed, asset.m_DataKey);

		for (const uint32_t& assetId : asset.m_Dependencies)
		{
			Ares::CombineHash<uint32_t>(seed, assetId);
		}

		return seed;
	}

	size_t hash<Ares::Asset>::operator()(const Ares::Ref<Ares::Asset>& asset) const
	{
		size_t seed = asset->m_Type.hash_code();

		if (asset->m_Filepath != nullptr)
			Ares::CombineHash<const char*>(seed, asset->m_Filepath.data());
		else if (asset->m_DataKey)
			Ares::CombineHash<uint32_t>(seed, asset->m_DataKey);

		for (const uint32_t& assetId : asset->m_Dependencies)
		{
			Ares::CombineHash<uint32_t>(seed, assetId);
		}

		return seed;
	}

}