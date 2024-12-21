#include <arespch.h>

#include "Engine/Data/Asset.h"

namespace Ares {

	Ref<Asset> Asset::Create(
		const std::type_index& type,
		const AssetState& state,
		const std::string& filepath,
		const std::vector<uint32_t>& dependencies,
		const void* rawData,
		const size_t& rawDataSize
	)
	{
		// CreateRef (std::make_shared) doesn't have access to private
		// constructors, so we wrap a raw pointer with a smart pointer
		// instead.
		return Ref<Asset>(new Asset(type, state, filepath, dependencies, rawData, rawDataSize));
	}

	Asset::Asset(
		const std::type_index& type,
		const AssetState& state,
		const std::string& filepath,
		const std::vector<uint32_t>& dependencies,
		const void* rawData,
		const size_t& rawDataSize
	)
		: m_Name(""),
		m_Filepath(filepath),
		m_TypeName(Utility::Type::ExtractClassName(type.name())),
		m_Type(type),
		m_Dependencies(dependencies),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(state),
		m_RawData(const_cast<void*>(rawData)),
		m_RawDataSize(rawDataSize)
	{
	}

	Asset::Asset()
		: m_Name(""),
		m_Filepath(""),
		m_TypeName(""),
		m_Type(typeid(void)),
		m_Dependencies({}),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(AssetState::None),
		m_RawData(nullptr),
		m_RawDataSize(0)
	{
	}

	Asset::~Asset()
	{
		m_Asset = nullptr;
	}

	const std::string Asset::GetStateString() const
	{
		std::shared_lock lock(m_AssetMutex);
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

	const size_t Asset::GetHash() const
	{
		std::shared_lock lock(m_AssetMutex);
		size_t hash = 0;

		// Hash type
		CombineHash<std::type_index>(hash, m_Type);

		// Hash filepath (if not empty)
		if (!m_Filepath.empty())
		{
			CombineHash<std::string>(hash, m_Filepath);
		}

		// Hash dependency ids (if not empty)
		for (uint32_t id : m_Dependencies)
		{
			CombineHash<uint32_t>(hash, id);
		}

		// Hash raw data (if not empty)
		if (m_RawData && m_RawDataSize != 0) {
			CombineHash<std::string_view>(hash, std::string_view(static_cast<const char*>(m_RawData), m_RawDataSize));
		}

		return hash;
	}

	void Asset::SetName(const std::string& name)
	{
		std::unique_lock lock(m_AssetMutex);
		m_Name = name;
	}

	void Asset::SetState(const AssetState& state)
	{
		std::unique_lock lock(m_AssetMutex);
		m_State = state;
	}

	void Asset::SetAssetId(const uint32_t& id)
	{
		std::unique_lock lock(m_AssetMutex);
		m_AssetId = id;
	}

	void Asset::SetAsset(const Ref<AssetBase>& asset)
	{
		std::unique_lock lock(m_AssetMutex);
		m_Asset = asset;
	}

}