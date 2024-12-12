#pragma once
#include <arespch.h>

#include "Engine/Core/Core.h"
#include "Engine/Data/Asset.h"
#include "Engine/Events/Event.h"

namespace Ares {

	class AssetLoadedEvent : public Event
	{
	public:
		AssetLoadedEvent(const std::string& filepath, bool loaded, Ref<Asset> asset = nullptr, const std::string& message = "")
			: m_Filepath(filepath), m_Loaded(loaded), m_Asset(asset), m_Message(message)
		{
		}

		inline const std::string& GetFilepath() const { return m_Filepath; }
		inline const std::string& GetMessage() const { return m_Message; }
		inline bool IsLoaded() const { return m_Loaded; }
		inline Ref<Asset> GetAsset() const { return m_Asset; }

		EVENT_CLASS_TYPE(AssetLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		std::string m_Filepath;
		std::string m_Message;
		bool m_Loaded;
		Ref<Asset> m_Asset;
	};

}