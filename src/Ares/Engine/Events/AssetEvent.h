#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetEvent : public Event
	{
	public:
		AssetEvent(Ref<AssetInfo> assetInfo, const std::string& message)
			: m_AssetInfo(assetInfo), m_Message(message)
		{
			if (assetInfo)
			{
				m_Name = assetInfo->Name;

				if (assetInfo->HasFilepath)
					m_Filepath = assetInfo->Filepath;
				else
					m_Filepath = "N/A";

				m_IsLoaded = m_AssetInfo->State == AssetState::Loaded ? true : false;
				m_LoadedStatus = LoadedStatus();
			}
			else
			{
				m_Name = "N/A";
				m_Filepath = "N/A";
				m_IsLoaded = false;
				m_LoadedStatus = "Failed";
			}
		}

		inline const std::string& GetStoreName() const { return m_Name; }
		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const std::string& GetMessage() const { return m_Message; }
		inline bool IsLoaded() const { return m_IsLoaded; }
		template <typename T>
		inline Ref<T> GetAsset() const {
			if (m_IsLoaded && m_AssetInfo)
				return static_pointer_cast<T>(m_AssetInfo->Asset);
			else
				return nullptr;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AssetLoadedEvent: " << m_Name << " Load Status - [" << m_LoadedStatus << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AssetUpdate)
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
		std::string m_Name;
		std::string m_Filepath;
		std::string m_LoadedStatus;
		bool m_IsLoaded;
	};

}