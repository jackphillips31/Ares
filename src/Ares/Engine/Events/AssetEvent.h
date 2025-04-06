#pragma once
#include "Engine/Events/Event.h"

namespace Ares {

	class Asset;
	enum class AssetState : uint8_t;

	class AssetBaseEvent : public Event
	{
	public:
		inline String GetEventMessage() const { return m_Message; }
		inline StringView GetAssetName() const { return m_AssetName; }
		inline StringView GetAssetFilepath() const { return m_AssetFilepath; }
		inline StringView GetAssetStateString() const { return m_AssetStateString; }
		inline AssetState GetAssetState() const { return m_AssetState; }
		inline const Ref<Asset>& GetAsset() const { return m_Asset; }
		template <typename AssetType>
		inline AssetType* GetRawAsset() const;

	protected:
		inline void Initialize(const Ref<Asset>& asset, StringView message);

	protected:
		String m_Message;
		StringView m_AssetName;
		StringView m_AssetFilepath;
		StringView m_AssetStateString;
		AssetState m_AssetState;
		Ref<Asset> m_Asset;
	};

	class AssetStagedEvent : public AssetBaseEvent
	{
	public:
		AssetStagedEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetStaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetStagedEvent)
	};

	class AssetLoadingEvent : public AssetBaseEvent
	{
	public:
		AssetLoadingEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetLoading)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetLoadingEvent)
	};

	class AssetLoadedEvent : public AssetBaseEvent
	{
	public:
		AssetLoadedEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetLoadedEvent)
	};

	class AssetFailedEvent : public AssetBaseEvent
	{
	public:
		AssetFailedEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetFailed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetFailedEvent)
	};

	class AssetUnloadedEvent : public AssetBaseEvent
	{
	public:
		AssetUnloadedEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetUnloaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetUnloadedEvent)
	};

	class AssetUnstagedEvent : public AssetBaseEvent
	{
	public:
		AssetUnstagedEvent(const Ref<Asset>& asset, StringView message = "");

		EVENT_CLASS_TYPE(AssetUnstaged)
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryAsset)
	private:
		EVENT_CLASS_CLONE(AssetUnstagedEvent)
	};

}