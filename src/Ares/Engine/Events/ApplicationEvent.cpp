#include <arespch.h>
#include "Engine/Events/ApplicationEvent.h"

#include <fmt/core.h>
#include <fmt/chrono.h>

namespace Ares {

	WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height, uint32_t clientWidth, uint32_t clientHeight)
		: m_Width(width), m_Height(height), m_ClientWidth(clientWidth), m_ClientHeight(clientHeight)
	{
	}

	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	WindowMovedEvent::WindowMovedEvent(int32_t x, int32_t y)
		: m_XPos(x), m_YPos(y)
	{
	}

	std::string WindowMovedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowMoveEvent: " << m_XPos << ", " << m_YPos;
		return ss.str();
	}

	WindowFocusEvent::WindowFocusEvent()
		: m_Timestamp(std::chrono::system_clock::now())
	{
	}

	std::string WindowFocusEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowFocusEvent: " << GetFormattedTimestamp();
		return ss.str();
	}

	std::string WindowFocusEvent::GetFormattedTimestamp() const
	{
		return fmt::format("{:%Y-%m-%d %H:%M:%S}", m_Timestamp);
	}

	WindowLostFocusEvent::WindowLostFocusEvent()
		: m_Timestamp(std::chrono::system_clock::now())
	{
	}

	std::string WindowLostFocusEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowLostFocusEvent: " << GetFormattedTimestamp();
		return ss.str();
	}

	std::string WindowLostFocusEvent::GetFormattedTimestamp() const
	{
		return fmt::format("{:%Y-%m-%d %H:%M:%S}", m_Timestamp);
	}

}