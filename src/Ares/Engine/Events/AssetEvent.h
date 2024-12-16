#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetEvent : public Event
	{
	public:
		AssetEvent(const Ref<Asset>& asset, const std::string& message)
			: m_Asset(asset), m_Message(message)
		{
			if (asset != nullptr)
			{
				m_Name = asset->GetName();

				if (asset->HasFilepath())
					m_Filepath = asset->GetFilepath();
				else
					m_Filepath = "N/A";

				m_AssetState = asset->GetState();
				m_IsLoaded = asset->GetState() == AssetState::Loaded ? true : false;
				m_LoadedStatus = LoadedStatus();
			}
			else
			{
				m_Name = "N/A";
				m_Filepath = "N/A";
				m_AssetState = AssetState::None;
				m_IsLoaded = false;
				m_LoadedStatus = "Failed";
			}
		}
		~AssetEvent()
		{
			m_Asset = nullptr;
		}

		AssetEvent& operator=(const AssetEvent&) = delete;

		inline const std::string& GetStoreName() const { return m_Name; }
		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const std::string& GetMessage() const { return m_Message; }
		inline const std::string& GetLoadedStatus() const { return m_LoadedStatus; }
		inline const AssetState& GetAssetState() const { return m_AssetState; }
		inline bool IsLoaded() const { return m_IsLoaded; }
		inline const Ref<Asset>& GetAssetWrapper() const { return m_Asset; }
		template <typename T>
		inline Ref<T> GetAsset() const {
			if (m_IsLoaded && m_Asset)
				return m_Asset->GetAsset<T>();
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
			switch (m_Asset->GetState())
			{
			case AssetState::None: return "None";
			case AssetState::Staged: return "Staged";
			case AssetState::Loading: return "Loading";
			case AssetState::Loaded: return "Loaded";
			case AssetState::Failed: return "Failed";
			}

			return "Unknown Asset State!";
		}

	private:
		Ref<Asset> m_Asset;
		std::string m_Message;
		std::string m_Name;
		std::string m_Filepath;
		std::string m_LoadedStatus;
		AssetState m_AssetState;
		bool m_IsLoaded;
	};

}