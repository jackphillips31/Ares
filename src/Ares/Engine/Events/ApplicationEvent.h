#pragma once

#include "Engine/Events/Event.h"

namespace Ares {
	
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height, uint32_t clientWidth, uint32_t clientHeight)
			: m_Width(width), m_Height(height), m_ClientWidth(clientWidth), m_ClientHeight(clientHeight)
		{
		}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline uint32_t GetClientWidth() const { return m_ClientWidth; }
		inline uint32_t GetClientHeight() const { return m_ClientHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t m_Width, m_Height, m_ClientWidth, m_ClientHeight;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int32_t x, int32_t y)
			: m_XPos(x), m_YPos(y)
		{
		}

		inline int32_t GetXPos() const { return m_XPos; }
		inline int32_t GetYPos() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMoveEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int32_t m_XPos, m_YPos;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}