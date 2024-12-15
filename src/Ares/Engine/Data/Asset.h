#pragma once
#include <arespch.h>

#include "Engine/Utility/Type.h"

namespace Ares {

	enum class AssetState : uint8_t
	{
		Unloaded = 0,
		Loading,
		Loaded,
		Failed
	};

	class Asset
	{
	public:
		virtual ~Asset() = default;
		virtual const std::string& GetName() const = 0;

		inline const uint32_t GetAssetId() const { return m_AssetId; }

	public:
		friend class AssetManager;

	private:
		inline void SetAssetId(uint32_t id) { m_AssetId = id; }

	private:
		uint32_t m_AssetId;
	};

	struct AssetInfo
	{
		std::string Name;
		std::string Filepath;
		std::string TypeName;
		bool HasFilepath;
		uint32_t AssetId;
		Ref<Asset> Asset;
		AssetState State;

		AssetInfo(const Ref<Ares::Asset>& asset, const std::string& typeName, const AssetState& state = AssetState::Unloaded, const std::string& filepath = "")
			: Asset(asset), Filepath(filepath), State(state)
		{
			Name = asset->GetName();
			AssetId = asset->GetAssetId();

			if (Filepath == "")
				HasFilepath = false;
			else
				HasFilepath = true;

			TypeName = Utility::Type::ExtractClassName(typeName);
		}
		AssetInfo()
			: Name(""), Filepath(""), TypeName(""), HasFilepath(false), AssetId(0), Asset(nullptr), State(AssetState::Unloaded)
		{
		}
	};

}