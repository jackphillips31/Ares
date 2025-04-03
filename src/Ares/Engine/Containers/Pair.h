/**
 * @file Pair.h
 * @brief Defines a convenient alias for an EASTL pair, a lightweight container for storing two related values.
 * 
 * @details **This file provides**:
 * @li [Pair<Type1, Type2](#Ares::Pair) -> A lightweight container for storing two related values.
 * @li [MakePair(Type1, Type2)](#Ares::MakePair) -> A helper function for constructing [Pair](#Ares::Pair)
 * objects with automatic type deduction.
 */
#pragma once
#include <EASTL/utility.h>

namespace Ares {

	/**
	 * @typedef Pair
	 * @brief Alias for `std::pair`, a simple container for holding two values.
	 * 
	 * @details **Features**:
	 * @li Stores two values of potentially different types.
	 * @li Provides **structured return values** from functions.
	 * @li Supports **comparison operators** for easy sorting and lookup.
	 * @li Works with **EASTL containers** like [HashMap](#Ares::HashMap) and [Vector](#Ares::Vector).
	 * 
	 * @tparam Type1 Type of the first value.
	 * @tparam Type2 Type of the second value.
	 * 
	 * @note Use [MakePair](#Ares::MakePair) for convenient pair creation without specifying template arguments.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type1, typename Type2>
	using Pair = eastl::pair<Type1, Type2>;

	/**
	 * @brief Creates a [Pair](#Ares::Pair) with automatically deduced types.
	 * 
	 * @details **Features**:
	 * @li **Automatic Type Deduction** -> Eliminates the need to specify [Pair](#Ares::Pair) template arguments.
	 * @li **Convenient Syntax** -> Simplifies pair initialization.
	 * @li **Compatible with EASTL Containers** -> Works seamlessly with [HashMap](#Ares::HashMap),
	 * [Vector](#Ares::Vector), and others.
	 * 
	 * @tparam Type1 Type of the first value (deduced automatically).
	 * @tparam Type2 Type of the second value (deduced automatically).
	 * 
	 * @return A [Pair<Type1, Type2>](#Ares::Pair) containing the given values.
	 * 
	 * @note Prefer [MakePair](#Ares::MakePair) over explicit [Pair<Type1, Type2>](#Ares::Pair) construction for readability.
	 * 
	 * @ingroup aliases
	 */
	template <typename Type1, typename Type2>
	constexpr auto MakePair(Type1&& first, Type2 second) -> Pair<Type1, Type2>
	{
		return eastl::make_pair(eastl::forward<Type1>(first), eastl::forward<Type2>(second));
	}

}