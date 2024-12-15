#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetLoadedEvent : public Event
	{
	public:
		AssetLoadedEvent(Ref<AssetInfo> assetInfo, const std::string& message)
			: m_AssetInfo(assetInfo), m_Message(message)
		{
		}

		inline const std::string& GetStoreName() const { return m_AssetInfo->Name; }
		inline const std::string& GetFilepath() const { return m_AssetInfo->Filepath; }
		inline const std::string& GetMessage() const { return m_Message; }
		inline bool IsLoaded() const { return m_AssetInfo->State == AssetState::Loaded ? true : false; }
		template <typename T>
		inline Ref<T> GetAsset() const { return static_pointer_cast<T>(m_AssetInfo->Asset); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AssetLoadedEvent: " << m_AssetInfo->Name << " Load Status - [" << LoadedStatus() << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		std::string LoadedStatus() const
		{
			switch (m_AssetInfo->State)
			{
			case AssetState::Unloaded: return "Unloaded";
			case AssetState::Loading: return "Loading";
			case AssetState::Loaded: return "Loaded";
			case AssetState::Failed: return "Failed";
			}

			return "Unknown Asset State!";
		}

	private:
		Ref<AssetInfo> m_AssetInfo;
		std::string m_Message;
	};

}