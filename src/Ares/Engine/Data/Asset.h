/**
 * @file Asset.h
 * @brief Defines the Asset and AssetBase classes and related types for managing assets in the Ares Engine.
 * 
 * @details This file includes the declaration of classes and enums that facilitate
 * asset management, such as loading, tracking, and storing various asset types.
 * It also includes thread-safe access mechanisms.
 */
#pragma once
#include <shared_mutex>
#include <typeindex>

namespace Ares {

	class AssetManager;
	using MemoryDataKey = uint32_t;

	/**
	 * @enum AssetState
	 * @brief Represents the state of an asset during its lifecycle.
	 */
	enum class AssetState : uint8_t
	{
		None = 0,		///< Asset is not initialized.
		Staged,			///< Asset is staged and ready for loading.
		Loading,		///< Asset is currently being loaded.
		Loaded,			///< Asset has been successfully loaded.
		Failed			///< Asset failed to load.
	};

	/**
	 * @class AssetBase
	 * @brief Base class for all assets in the Ares engine.
	 * 
	 * @details Provides a pure virtual interface for accessing the name of an asset.
	 */
	class AssetBase
	{
	public:
		AssetBase() = default;
		virtual ~AssetBase() = default;

		/**
		 * @brief Get the name of the asset.
		 * 
		 * @return A constant reference to the asset name.
		 */
		virtual const std::string& GetName() const = 0;

		// Delete copy constructor and operator
		AssetBase(const AssetBase&) = delete;
		AssetBase& operator=(const AssetBase&) = delete;
	};

	/**
	 * @class Asset
	 * @brief Represents a managed asset in the Ares engine.
	 * 
	 * @details This class provides methods for managing asset properties and ensuring
	 * thread-safe access to asset data. Assets are created and managed by the AssetManager.
	 * 
	 * @note Only the AssetManager can instantiate Asset objects.
	 */
	class Asset
	{
	private:
		/**
		 * @brief Creates an Asset instance.
		 * 
		 * @param type The type index of the asset.
		 * @param state The current state of the asset.
		 * @param filepath The filepath of the asset.
		 * @param dependencies A list of dependency IDs.
		 * @param dataKey The key for the asset's memory data.
		 * @return A Ref of an Asset object.
		 */
		static Ref<Asset> Create(
			const std::type_index& type,
			const AssetState state,
			const std::string& filepath,
			const std::vector<uint32_t>& dependencies,
			const MemoryDataKey dataKey
		);

		/**
		 * @brief Construct an Asset instance.
		 *
		 * @param type The type index of the asset.
		 * @param state The current state of the asset.
		 * @param filepath The filepath of the asset.
		 * @param dependencies A list of dependency IDs.
		 * @param dataKey The key for the asset's memory data.
		 */
		Asset(
			const std::type_index& type,
			const AssetState state,
			const std::string& filepath,
			const std::vector<uint32_t>& dependencies,
			const MemoryDataKey dataKey
		);

		/**
		 * @brief Private default constructor.
		 */
		Asset();

		friend class AssetManager;
		friend class MainThreadQueue;
		friend struct std::hash<Asset>;

	public:
		/**
		 * @brief Deleted copy constructor.
		 */
		Asset(const Asset&) = delete;

		/**
		 * @brief Deleted copy assignment operator.
		 */
		Asset& operator=(const Asset&) = delete;

		/**
		 * @brief Destructor.
		 */
		~Asset();

		/**
		 * @brief Get the name of the asset.
		 * 
		 * @return The asset's name as a string.
		 */
		inline std::string GetName() const { std::shared_lock lock(m_Mutex); return m_Name; }
		
		/**
		 * @brief Get the filepath of the asset.
		 * 
		 * @return The asset's file path as a string.
		 */
		inline std::string GetFilepath() const { std::shared_lock lock(m_Mutex); return m_Filepath; }
		
		/**
		 * @brief Get the type name of the asset.
		 * 
		 * @return The asset's type name as a string.
		 */
		inline std::string GetTypeName() const { std::shared_lock lock(m_Mutex); return m_TypeName; }
		
		/**
		 * @brief Get the type index of the asset.
		 * 
		 * @return The asset's type index.
		 */
		inline std::type_index GetType() const { std::shared_lock lock(m_Mutex); return m_Type; }
		
		/**
		 * @brief Get the dependencies of the asset.
		 * 
		 * @return A vector of dependency IDs.
		 */
		inline std::vector<uint32_t> GetDependencies() const { std::shared_lock lock(m_Mutex); return m_Dependencies; }
		
		/**
		 * @brief Get the asset's unique ID.
		 * 
		 * @return The asset's ID as a 32 bit unsigned integer.
		 */
		inline uint32_t GetAssetId() const { std::shared_lock lock(m_Mutex); return m_AssetId; }
		
		/**
		 * @brief Check if the asset has a filepath.
		 * 
		 * @return `true` if the asset has a filepath; otherwise, `false`.
		 */
		inline bool HasFilepath() const { std::shared_lock lock(m_Mutex); return !m_Filepath.empty(); }
		
		/**
		 * @brief Check if the asset is loaded.
		 * 
		 * @return `true` if the asset is loaded; otherwise, `false`.
		 */
		inline bool IsLoaded() const { std::shared_lock lock(m_Mutex); return m_State == AssetState::Loaded; }
		
		/**
		 * @brief Get the state of the asset.
		 * 
		 * @return The current state of the asset.
		 */
		inline AssetState GetState() const { std::shared_lock lock(m_Mutex); return m_State; }
		
		/**
		 * @brief Get the string representation of the asset's state.
		 * 
		 * @return A string describing the asset's state.
		 */
		std::string GetStateString() const;
		
		/**
		 * @brief Get the size of the asset's data.
		 * 
		 * @return The data size in bytes.
		 */
		size_t GetDataSize() const;

		/**
		 * @brief Get the underlying raw asset.
		 * 
		 * @tparam AssetType The type of the asset.
		 * @return A pointer to the raw asset object.
		 */
		template <typename AssetType>
		inline AssetType* GetAsset() const
		{
			std::shared_lock lock(m_Mutex);
			return static_cast<AssetType*>(m_Asset.get());
		}
		
		/**
		 * @brief Get the underlying asset base.
		 * 
		 * @return A constant Scope<AssetBase> reference to the asset data.
		 */
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
		mutable std::shared_mutex m_Mutex;			///< Mutex for thread-safe access.
		std::string m_Name;							///< Asset name.
		std::string m_Filepath;						///< Filepath of the asset.
		std::string m_TypeName;						///< Type name of the asset.
		std::type_index m_Type;						///< Type index of the asset.
		std::vector<uint32_t> m_Dependencies;		///< List of asset's dependencies in the form of IDs.
		uint32_t m_AssetId;							///< Unique ID of the asset.
		Scope<AssetBase> m_Asset;					///< Pointer to the underlying asset data.
		AssetState m_State;							///< Current state of the asset.
		MemoryDataKey m_DataKey;					///< Key for memory data.
	};

}