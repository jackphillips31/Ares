#include <arespch.h>
#include "Engine/Core/Timestep.h"

namespace Ares {

	TimestepNew::TimestepNew(uint64_t time)
		: m_Time(time)
	{
	}

	TimestepNew::operator uint64_t() const
	{
		return m_Time;
	}

	uint64_t TimestepNew::GetNanoseconds() const
	{
		return m_Time;
	}

	double TimestepNew::GetMilliseconds() const
	{
		return static_cast<double>(m_Time) / 1e6;
	}

	double TimestepNew::GetSeconds() const
	{
		return static_cast<double>(m_Time) / 1e9;
	}

}