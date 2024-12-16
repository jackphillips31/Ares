#include <arespch.h>

#include "Engine/Data/Asset.h"

namespace Ares {

	Asset::Asset(const Ref<AssetBase>& asset, const std::type_index& type, const AssetState& state, const std::string& filepath)
		: m_Name(asset->GetName()), m_Filepath(filepath), m_Type(type), m_AssetId(asset->GetAssetId()), m_Asset(asset), m_State(state)
	{
		m_TypeName = Utility::Type::ExtractClassName(type.name());

		if (filepath == "")
			m_HasFilepath = false;
		else
			m_HasFilepath = true;
	}

	Asset::Asset()
		: m_Name(""), m_Filepath(""), m_Type(typeid(void)), m_HasFilepath(false), m_AssetId(0), m_Asset(nullptr), m_State(AssetState::None)
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

	void Asset::SetNameAndFilepath(const std::string& name, const std::string& filepath)
	{
		m_Name = name;
		m_Filepath = filepath;
	}

	void Asset::SetState(const AssetState& state)
	{
		m_State = state;
	}

	void Asset::SetAsset(const Ref<AssetBase>& asset)
	{
		m_Asset = asset;
	}

}