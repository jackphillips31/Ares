#include <arespch.h>
#include "Engine/Data/Asset.h"

#include "Engine/Core/Utility.h"
#include "Engine/Data/DataBuffer.h"
#include "Engine/Data/MemoryDataProvider.h"

namespace Ares {

	Ref<Asset> Asset::Create(
		const std::type_index& type,
		const AssetState state,
		const std::string& filepath,
		const std::vector<uint32_t>& dependencies,
		const MemoryDataKey dataKey
	)
	{
		// CreateRef (std::make_shared) doesn't have access to private
		// constructors, so we wrap a raw pointer with a smart pointer
		// instead.
		return Ref<Asset>(new Asset(type, state, filepath, dependencies, dataKey));
	}

	Asset::Asset(
		const std::type_index& type,
		const AssetState state,
		const std::string& filepath,
		const std::vector<uint32_t>& dependencies,
		const MemoryDataKey dataKey
	)
		: m_Name(""),
		m_Filepath(filepath),
		m_TypeName(Utility::ExtractClassName(type)),
		m_Type(type),
		m_Dependencies(dependencies),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(state),
		m_DataKey(dataKey)
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

	std::string Asset::GetStateString() const
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

	void Asset::SetName(const std::string& name)
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