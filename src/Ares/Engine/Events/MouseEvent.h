#pragma once
#include "Engine/Events/Event.h"
#include "Engine/Input/MouseCodes.h"
#include "Engine/Input/MousePosition.h"

namespace Ares {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int32_t x, int32_t y)
			: m_MousePos(x, y)
		{
		}

		inline MousePosition GetPosition() const { return m_MousePos; }
		inline int32_t GetX() const { return m_MousePos.x; }
		inline int32_t GetY() const { return m_MousePos.y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePos.x << ", " << m_MousePos.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		EVENT_CLASS_CLONE(MouseMovedEvent)

	private:
		MousePosition m_MousePos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{
		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		EVENT_CLASS_CLONE(MouseScrolledEvent)

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(MouseCode button)
			: m_Button(button)
		{
		}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)

	private:
		EVENT_CLASS_CLONE(MouseButtonPressedEvent)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)

	private:
		EVENT_CLASS_CLONE(MouseButtonReleasedEvent)
	};

}