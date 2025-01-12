#pragma once
#include <shared_mutex>
#include <typeindex>

namespace Ares {

	class AssetManager;
	using MemoryDataKey = uint32_t;

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
		AssetBase() = default;
		virtual ~AssetBase() = default;

		// Core property
		virtual const std::string& GetName() const = 0;

		// Delete copy constructor and operator
		AssetBase(const AssetBase&) = delete;
		AssetBase& operator=(const AssetBase&) = delete;
	};

	class Asset
	{
	private:
		static Ref<Asset> Create(
			const std::type_index& type,
			const AssetState state,
			const std::string& filepath,
			const std::vector<uint32_t>& dependencies,
			const MemoryDataKey dataKey
		);
		Asset(
			const std::type_index& type,
			const AssetState state,
			const std::string& filepath,
			const std::vector<uint32_t>& dependencies,
			const MemoryDataKey dataKey
		);
		Asset();

		friend class AssetManager;
		friend class MainThreadQueue;
		friend struct std::hash<Asset>;

	public:
		// Delete copy constructor and copy assignment operator
		Asset(const Asset&) = delete;
		Asset& operator=(const Asset&) = delete;
		~Asset();

		// Getters
		inline std::string GetName() const { std::shared_lock lock(m_Mutex); return m_Name; }
		inline std::string GetFilepath() const { std::shared_lock lock(m_Mutex); return m_Filepath; }
		inline std::string GetTypeName() const { std::shared_lock lock(m_Mutex); return m_TypeName; }
		inline std::type_index GetType() const { std::shared_lock lock(m_Mutex); return m_Type; }
		inline std::vector<uint32_t> GetDependencies() const { std::shared_lock lock(m_Mutex); return m_Dependencies; }
		inline uint32_t GetAssetId() const { std::shared_lock lock(m_Mutex); return m_AssetId; }
		inline bool HasFilepath() const { std::shared_lock lock(m_Mutex); return !m_Filepath.empty(); }
		inline bool IsLoaded() const { std::shared_lock lock(m_Mutex); return m_State == AssetState::Loaded; }
		inline AssetState GetState() const { std::shared_lock lock(m_Mutex); return m_State; }
		std::string GetStateString() const;
		size_t GetDataSize() const;

		template <typename AssetType>
		inline AssetType* GetAsset() const
		{
			std::shared_lock lock(m_Mutex);
			return static_cast<AssetType*>(m_Asset.get());
		}
		inline const Scope<AssetBase>& GetAsset() const
		{
			std::shared_lock lock(m_Mutex);
			return m_Asset;
		}

	private:
		// Private setters for AssetManager to use
		void SetName(const std::string& name);
		void SetState(const AssetState state);
		void SetAssetId(const uint32_t id);
		void SetAsset(Scope<AssetBase>&& asset);
		void SetDataKey(const MemoryDataKey dataKey);

		// Private getters for AssetManager to use
		inline const MemoryDataKey GetDataKey() const { std::shared_lock lock(m_Mutex); return m_DataKey; }

		// Private methods for unloading and unstaging
		void Unload();
		void Unstage();

	private:
		mutable std::shared_mutex m_Mutex;
		std::string m_Name;
		std::string m_Filepath;
		std::string m_TypeName;
		std::type_index m_Type;
		std::vector<uint32_t> m_Dependencies;
		uint32_t m_AssetId;
		Scope<AssetBase> m_Asset;
		AssetState m_State;
		MemoryDataKey m_DataKey;
	};

}