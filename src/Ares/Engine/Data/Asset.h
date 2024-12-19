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
	};

	class AssetManager;

	class Asset
	{
	private:
		static Ref<Asset> Create(const std::type_index& type, const AssetState& state, const std::string& filepath, const std::vector<uint32_t>& dependencies);
		Asset(const std::type_index& type, const AssetState& state, const std::string& filepath, const std::vector<uint32_t>& dependencies);
		Asset();

		friend class AssetManager;

	public:
		// Delete copy constructor and copy assignment operator
		Asset(const Asset&) = delete;
		Asset& operator=(const Asset&) = delete;
		~Asset();

		// Getters
		inline const std::string& GetName() const { std::shared_lock lock(m_AssetMutex); return m_Name; }
		inline const std::string& GetFilepath() const { std::shared_lock lock(m_AssetMutex); return m_Filepath; }
		inline const std::string& GetTypeName() const { std::shared_lock lock(m_AssetMutex); return m_TypeName; }
		inline const std::type_index& GetType() const { std::shared_lock lock(m_AssetMutex); return m_Type; }
		inline const std::vector<uint32_t>& GetDependencies() const { std::shared_lock lock(m_AssetMutex); return m_Dependencies; }
		inline const uint32_t& GetAssetId() const { std::shared_lock lock(m_AssetMutex); return m_AssetId; }
		inline const bool HasFilepath() const { std::shared_lock lock(m_AssetMutex); return !m_Filepath.empty(); }
		inline const bool IsLoaded() const { std::shared_lock lock(m_AssetMutex); return m_State == AssetState::Loaded; }
		inline const AssetState& GetState() const { std::shared_lock lock(m_AssetMutex); return m_State; }
		const std::string GetStateString() const;
		const size_t GetHash() const;

		template <typename AssetType>
		inline const Ref<AssetType> GetAsset() const
		{
			std::shared_lock lock(m_AssetMutex);
			return static_pointer_cast<AssetType>(m_Asset);
		}
		inline const Ref<AssetBase>& GetAsset() const
		{
			std::shared_lock lock(m_AssetMutex);
			return m_Asset;
		}

	private:
		// Private setters for AssetManager to use
		void SetName(const std::string& name);
		void SetState(const AssetState& state);
		void SetAssetId(const uint32_t& id);
		void SetAsset(const Ref<AssetBase>& asset);

	private:
		std::string m_Name;
		std::string m_Filepath;
		std::string m_TypeName;
		std::type_index m_Type;
		std::vector<uint32_t> m_Dependencies;
		bool m_HasFilepath;
		uint32_t m_AssetId;
		Ref<AssetBase> m_Asset;
		AssetState m_State;
		mutable std::shared_mutex m_AssetMutex;
	};

}