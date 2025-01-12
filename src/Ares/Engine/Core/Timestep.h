#pragma once

namespace Ares {

	class Timestep
	{
	public:
		Timestep(double time = 0.0f)
			: m_Time(time)
		{
		}

		operator double() const { return m_Time; }

		double GetSeconds() const { return m_Time; }
		double GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		double m_Time;
	};
}