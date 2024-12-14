#pragma once

#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetLoadedEvent : public Event
	{
	public:
		AssetLoadedEvent(const std::string& name, bool loaded, Ref<Asset> asset = nullptr, const std::string& filepath = "n/a", const std::string & message = "n/a")
			: m_StoreName(name), m_Loaded(loaded), m_Asset(asset), m_Filepath(filepath), m_Message(message)
		{
		}

		inline const std::string& GetStoreName() const { return m_StoreName; }
		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const std::string& GetMessage() const { return m_Message; }
		inline bool IsLoaded() const { return m_Loaded; }
		template <typename T>
		inline Ref<T> GetAsset() const { return static_pointer_cast<T>(m_Asset); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AssetLoadedEvent: Load Status - [" << IsLoaded() << "], Storage Name - " << m_StoreName;
			return ss.str();
		}

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		std::string m_StoreName;
		std::string m_Filepath;
		std::string m_Message;
		bool m_Loaded;
		Ref<Asset> m_Asset;
	};

}