/**
 * @file Timestep.h
 * @brief Contains the Timestep class, which is used to represent the time elapsed in a frame.
 * 
 * @details The Timestep class provides a convenient way to handle time calculations such as seconds
 * and milliseconds. It can be used to measure time intervals, such as the time between frames in a game loop.
 */
#pragma once

namespace Ares {

	/**
	 * @class Timestep
	 * @brief Represents the time that has elapsed in a frame or time interval.
	 * 
	 * @details The Timestep class encapsulates a time value and provides methods for retrieving that value
	 * in different units such as seconds or milliseconds. It can be used for frame time or delta time
	 * calculations in game loops or simulations.
	 */
	class Timestep
	{
	public:
		/**
		 * @brief Constructs a Timestep object.
		 * 
		 * @param time The time value to initialize the Timestep object with. Defaults to `0.0f`.
		 */
		Timestep(double time = 0.0f)
			: m_Time(time)
		{
		}

		/**
		 * @brief Converts the Timestep to a double value.
		 * 
		 * @details Implicit conversion operator to double to get the time in seconds.
		 * 
		 * @return The time value in seconds.
		 */
		operator double() const { return m_Time; }

		/**
		 * @brief Returns the time in seconds.
		 * 
		 * @return The time in seconds as a double.
		 */
		double GetSeconds() const { return m_Time; }

		/**
		 * @brief Returns the time in milliseconds.
		 * 
		 * @return The time in milliseconds as a double.
		 */
		double GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		double m_Time;		///< The time value in seconds.
	};

}