/**
 * @file MousePosition.h
 * @brief This file defines a structure for storing mouse position.
 * 
 * @details This file contains the definition of the [MousePosition](#Ares::MousePosition)
 * struct, which holds the `X` and `Y` coordinates of the mouse pointer. It is used for
 * managing mouse position data in the application.
 */
#pragma once
#include <glm/vec2.hpp>

namespace Ares {

	/**
	 * @struct MousePosition
	 * @brief Represents the position of the mouse in 2D space, with x and y coordinates.
	 *
	 * @details This struct provides methods for easy manipulation of the mouse position and
	 * supports basic arithmetic operations such as addition, subtraction, and comparison.
	 * It can also be implicitly converted to a `glm::ivec2` for integration with GLM-based operations.
	 *
	 * @note The struct supports construction from an initializer list, as well as a `glm::ivec2` type.
	 */
	struct MousePosition
	{
		/**
		 * @brief The x-coordinate of the mouse position.
		 */
		int32_t x;

		/**
		 * @brief The y-coordinate of the mouse position.
		 */
		int32_t y;

		/**
		 * @brief Default constructor initializing the mouse position to (0, 0) or with specified x and y values.
		 *
		 * @param x The x-coordinate of the mouse position.
		 * @param y The y-coordinate of the mouse position.
		 */
		MousePosition(int32_t x = 0, int32_t y = 0) : x(x), y(y) {}

		/**
		 * @brief Constructor that initializes the mouse position from a `glm::ivec2` object.
		 *
		 * @param vec A `glm::ivec2` representing the x and y coordinates of the mouse position.
		 */
		MousePosition(const glm::ivec2& vec) : x(vec.x), y(vec.y) {}

		/**
		 * @brief Constructor that initializes the mouse position from an initializer list containing two integers.
		 *
		 * @param list An initializer list with two integers, corresponding to the x and y coordinates.
		 */
		MousePosition(std::initializer_list<int32_t> list) { auto it = list.begin(); x = *it; y = *(++it); }

		/**
		 * @brief Implicitly converts this MousePosition object to a `glm::ivec2`.
		 *
		 * @details This operator allows a MousePosition object to be implicitly converted
		 * to a `glm::ivec2` object, with the `x` and `y` members mapped directly to
		 * the `x` and `y` components of the `glm::ivec2`.
		 *
		 * @return A `glm::ivec2` object initialized with the `x` and `y` values
		 * of this MousePosition object.
		 *
		 * @note This operator enables seamless interoperability with the GLM library,
		 * allowing MousePosition to be used in contexts where `glm::ivec2` is expected.
		 */
		inline operator glm::ivec2() const { return glm::ivec2(x, y); }

		/**
		 * @brief Adds another MousePosition to this MousePosition.
		 *
		 * @param other The other MousePosition to add.
		 * @return A new MousePosition representing the result of the addition.
		 */
		inline MousePosition operator+(const MousePosition& other) const { return MousePosition(x + other.x, y + other.y); }

		/**
		 * @brief Subtracts another MousePosition from this MousePosition.
		 *
		 * @param other The other MousePosition to subtract.
		 * @return A new MousePosition representing the result of the subtraction.
		 */

		inline MousePosition operator-(const MousePosition& other) const { return MousePosition(x - other.x, y - other.y); }

		/**
		 * @brief Assigns the values of a `glm::ivec2` object to this MousePosition object.
		 *
		 * @details This operator updates the `x` and `y` members of the current MousePosition instance
		 * to match the corresponding values of the provided `glm::ivec2` object.
		 *
		 * @param vec A `glm::ivec2` object containing the new `x` and `y` values.
		 * @return A reference to the current MousePosition object after assignment.
		 *
		 * @note This operator allows assignment from a `glm::ivec2` object, enabling seamless
		 * integration with the GLM library for vector operations.
		 */
		inline MousePosition& operator=(const glm::ivec2& vec) { x = vec.x; y = vec.y; return *this; }

		/**
		 * @brief Adds another MousePosition to this MousePosition (in-place).
		 *
		 * @param other The other MousePosition to add.
		 * @return The updated MousePosition object.
		 */
		inline MousePosition& operator+=(const MousePosition& other) { x += other.x; y += other.y; return *this; }

		/**
		 * @brief Subtracts another MousePosition from this MousePosition (in-place).
		 *
		 * @param other The other MousePosition to subtract.
		 * @return The updated MousePosition object.
		 */
		inline MousePosition& operator-=(const MousePosition& other) { x -= other.x; y -= other.y; return *this; }

		/**
		 * @brief Checks if two MousePosition objects are equal.
		 *
		 * @param other The other MousePosition to compare.
		 * @return `true` if the two positions are equal; otherwise, `false`.
		 */
		inline bool operator==(const MousePosition& other) const { return x == other.x && y == other.y; }

		/**
		 * @brief Checks if two MousePosition objects are not equal.
		 *
		 * @param other The other MousePosition to compare.
		 * @return `true` if the two positions are not equal; otherwise, `false`.
		 */
		inline bool operator!=(const MousePosition& other) const { return !(*this == other); }
	};

}