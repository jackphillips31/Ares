#include <arespch.h>

#include "Engine/Data/Asset.h"

namespace Ares {

	Ref<Asset> Asset::Create(const std::type_index& type, const AssetState& state, const std::string& filepath, const std::vector<uint32_t>& dependencies)
	{
		// CreateRef (std::make_shared) doesn't have access to private
		// constructors, so we wrap a raw pointer with a smart pointer
		// instead.
		return Ref<Asset>(new Asset(type, state, filepath, dependencies));
	}

	Asset::Asset(const std::type_index& type, const AssetState& state, const std::string& filepath, const std::vector<uint32_t>& dependencies)
		: m_Name(""),
		m_Filepath(filepath),
		m_TypeName(Utility::Type::ExtractClassName(type.name())),
		m_Type(type),
		m_Dependencies(dependencies),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(state)
	{
	}

	Asset::Asset()
		: m_Name(""),
		m_Filepath(""),
		m_TypeName(""),
		m_Type(typeid(void)),
		m_HasFilepath(false),
		m_Dependencies({}),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(AssetState::None)
	{
	}

	Asset::~Asset()
	{
		m_Asset = nullptr;
	}

	const std::string Asset::GetStateString() const
	{
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
		size_t hash = 0;

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

		// Hash type
		CombineHash<std::type_index>(hash, m_Type);

		return hash;
	}

	void Asset::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void Asset::SetState(const AssetState& state)
	{
		m_State = state;
	}

	void Asset::SetAssetId(const uint32_t& id)
	{
		m_AssetId = id;
	}

	void Asset::SetAsset(const Ref<AssetBase>& asset)
	{
		m_Asset = asset;
	}

}