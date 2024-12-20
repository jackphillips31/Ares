#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetBaseEvent : public Event
	{
	public:
		inline const std::string& GetEventMessage() const { return m_Message; }
		inline const std::string& GetAssetName() const { return m_AssetName; }
		inline const std::string& GetAssetFilepath() const { return m_AssetFilepath; }
		inline const std::string& GetAssetStateString() const { return m_AssetStateString; }
		inline const AssetState& GetAssetState() const { return m_AssetState; }
		inline const Ref<Asset>& GetAsset() const { return m_Asset; }
		template <typename AssetType>
		inline const Ref<AssetType> GetAsset() const
		{
			if (m_AssetState == AssetState::Loaded)
				return m_Asset->GetAsset<AssetType>();
			else
				return nullptr;
		}

		EVENT_CLASS_TYPE(AssetUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	protected:
		inline void Initialize(const Ref<Asset>& asset, const std::string& message)
		{
			m_Asset = asset;
			m_Message = message;
			m_AssetName = asset->GetName();
			m_AssetFilepath = asset->GetFilepath();
			m_AssetStateString = ToStateString(asset->GetState());
			m_AssetState = asset->GetState();
		}
		inline const std::string ToStateString(AssetState state) const
		{
			switch (state)
			{
			case AssetState::None: return "None";
			case AssetState::Staged: return "Staged";
			case AssetState::Loading: return "Loading";
			case AssetState::Loaded: return "Loaded";
			case AssetState::Failed: return "Failed";
			}

			return "Unknown Asset State!";
		}

	protected:
		std::string m_Message;
		std::string m_AssetName;
		std::string m_AssetFilepath;
		std::string m_AssetStateString;
		AssetState m_AssetState;
		Ref<Asset> m_Asset;
	};

	class AssetStagedEvent : public AssetBaseEvent
	{
	public:
		AssetStagedEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetStaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetLoadingEvent : public AssetBaseEvent
	{
	public:
		AssetLoadingEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetLoading)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetLoadedEvent : public AssetBaseEvent
	{
	public:
		AssetLoadedEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetFailedEvent : public AssetBaseEvent
	{
	public:
		AssetFailedEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetFailed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetUnloadedEvent : public AssetBaseEvent
	{
	public:
		AssetUnloadedEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetUnloaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetUnstagedEvent : public AssetBaseEvent
	{
	public:
		AssetUnstagedEvent(const Ref<Asset>& asset, const std::string& message = "")
		{
			Initialize(asset, message);
		}

		EVENT_CLASS_TYPE(AssetUnstaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}