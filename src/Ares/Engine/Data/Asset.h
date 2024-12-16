#pragma once

#include "Engine/Utility/Type.h"

namespace Ares {

	enum class AssetState : uint8_t
	{
		None = 0,
		Staged,
		Loading,
		Loaded,
		Failed
	};

	class AssetBase
	{
	public:
		virtual ~AssetBase() = default;
		virtual const std::string& GetName() const = 0;

		inline const uint32_t GetAssetId() const { return m_AssetId; }

	public:
		friend class AssetManager;

	private:
		inline void SetAssetId(uint32_t id) { m_AssetId = id; }

	private:
		uint32_t m_AssetId;
	};

	class Asset
	{
	public:
		Asset(const Ref<AssetBase>& asset, const std::type_index& type, const AssetState& state = AssetState::None, const std::string& filepath = "");
		Asset();

		~Asset();

		// Delete copy constructor and copy assignment operator
		Asset(const Asset&) = delete;
		Asset& operator=(const Asset&) = delete;

		// Getters
		const std::string& GetName() const;
		const std::string& GetFilepath() const;
		const std::string& GetTypeName() const;
		const std::type_index& GetType() const;
		const uint32_t& GetAssetId() const;
		const bool& HasFilepath() const;
		const AssetState& GetState() const;

		template <typename T>
		inline const Ref<T> GetAsset() const
		{
			return static_pointer_cast<T>(m_Asset);
		}
		inline const Ref<AssetBase>& GetAsset() const
		{
			return m_Asset;
		}

	public:
		friend class AssetManager;

	private:
		// Private setters for AssetManager to use
		void SetNameAndFilepath(const std::string& name, const std::string& filepath);
		void SetState(const AssetState& state);
		void SetAsset(const Ref<AssetBase>& asset);

	private:
		std::string m_Name;
		std::string m_Filepath;
		std::string m_TypeName;
		std::type_index m_Type;
		bool m_HasFilepath;
		uint32_t m_AssetId;
		Ref<AssetBase> m_Asset;
		AssetState m_State;
	};

}