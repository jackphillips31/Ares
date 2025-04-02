/**
 * @file Function.h
 * @brief Defines a convenient alias for EASTL function objects.
 * @details This file provides a type alias for function pointers or callable objects,
 * improving readability and maintainability. Using this alias allows the engine to
 * easily swap between `eastl::function` and raw function pointers for performance
 * tuning.
 * 
 * @ingroup aliases
 */
#pragma once
#include <EASTL/functional.h>

namespace Ares {

	/**
	 * @typedef Function
	 * @brief Alias for `eastl::function`.
	 * 
	 * @details Features:
	 * @li **Allocation-free** for callables <= 64 bytes (configurable).
	 * @li **Exception-safe** invocation.
	 * @li **Move-only** semantics.
	 * @li Compatible with lambdas, function pointers, and `std::bind` results.
	 * 
	 * **Performance Characteristics**:
	 * |Operation|Cost|
	 * |-|-|
	 * |Construction|O(1) + potential copy|
	 * |Invocation|1 indirect call|
	 * |Move|1 pointer call|
	 * 
	 * **Example Usage**:
	 * ```cpp
	 * Function<void(Asset*)> callback = [](Asset* a)
	 * {
	 *     ProcessAsset(a);
	 * };
	 * callback(&asset);
	 * ```
	 * 
	 * @tparam T Function signature (e.g., `void(int, float)`)
	 * 
	 * @warning Not thread-safe for concurrent invocations.
	 * @ingroup aliases
	 */
	template <typename T>
	using Function = eastl::function<T>;

}