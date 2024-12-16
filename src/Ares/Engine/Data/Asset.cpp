#include <arespch.h>

#include "Engine/Data/Asset.h"

namespace Ares {

	Asset::Asset(const std::type_index& type, const AssetState& state, const std::string& filepath)
		: m_Name(""),
		m_Filepath(filepath),
		m_TypeName(Utility::Type::ExtractClassName(type.name())),
		m_Type(type),
		m_Dependencies({}),
		m_AssetId(0),
		m_Asset(nullptr),
		m_State(state)
	{
		if (!filepath.empty())
			m_HasFilepath = true;
		else
			m_HasFilepath = false;
	}

	Asset::Asset(const std::type_index& type, const AssetState& state, const std::vector<uint32_t>& dependencies)
		: m_Name(""),
		m_Filepath(""),
		m_TypeName(Utility::Type::ExtractClassName(type.name())),
		m_Type(type),
		m_Dependencies(dependencies),
		m_HasFilepath(false),
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

	const std::string& Asset::GetName() const
	{
		return m_Name;
	}

	const std::string& Asset::GetFilepath() const
	{
		return m_Filepath;
	}

	const std::string& Asset::GetTypeName() const
	{
		return m_TypeName;
	}

	const std::type_index& Asset::GetType() const
	{
		return m_Type;
	}

	const std::vector<uint32_t>& Asset::GetDependencies() const
	{
		return m_Dependencies;
	}

	const uint32_t& Asset::GetAssetId() const
	{
		return m_AssetId;
	}

	const bool& Asset::HasFilepath() const
	{
		return m_HasFilepath;
	}

	const AssetState& Asset::GetState() const
	{
		return m_State;
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

	void Asset::SetNameAndFilepath(const std::string& name, const std::string& filepath)
	{
		m_Name = name;
		m_Filepath = filepath;
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
		m_Name = asset->GetName();
		m_AssetId = asset->GetAssetId();
	}

}