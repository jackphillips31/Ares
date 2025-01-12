#pragma once
#include "Engine/Events/Event.h"

namespace Ares {

	class Asset;
	enum class AssetState : uint8_t;

	class AssetBaseEvent : public Event
	{
	public:
		inline std::string GetEventMessage() const { return m_Message; }
		inline std::string GetAssetName() const { return m_AssetName; }
		inline std::string GetAssetFilepath() const { return m_AssetFilepath; }
		inline std::string GetAssetStateString() const { return m_AssetStateString; }
		inline AssetState GetAssetState() const { return m_AssetState; }
		inline const Ref<Asset>& GetAsset() const { return m_Asset; }
		template <typename AssetType>
		inline AssetType* GetRawAsset() const;

		EVENT_CLASS_TYPE(AssetUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	protected:
		inline void Initialize(const Ref<Asset>& asset, const std::string& message);

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
		AssetStagedEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetStaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetLoadingEvent : public AssetBaseEvent
	{
	public:
		AssetLoadingEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetLoading)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetLoadedEvent : public AssetBaseEvent
	{
	public:
		AssetLoadedEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetFailedEvent : public AssetBaseEvent
	{
	public:
		AssetFailedEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetFailed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetUnloadedEvent : public AssetBaseEvent
	{
	public:
		AssetUnloadedEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetUnloaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AssetUnstagedEvent : public AssetBaseEvent
	{
	public:
		AssetUnstagedEvent(const Ref<Asset>& asset, const std::string& message = "");

		EVENT_CLASS_TYPE(AssetUnstaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}