/**
 * @file StringView.h
 * @brief Defines a StringView alias for non-owning string references.
 * 
 * @details This file defines [StringView](#Ares::StringView), an alias for `eastl::basic_string_view<char>`.
 * It provides a lightweight, non-owning view into a string, making it ideal for efficient string manipulation
 * without unnecessary allocations or copies.
 * 
 * @ingroup containers
 */
#pragma once
#include <EASTL/string_view.h>

namespace Ares {

	/**
	 * @typedef StringView
	 * @brief Alias for `eastl::basic_string_view<char>`, providing a non-owning string reference.
	 * 
	 * @details **Features**:
	 * @li Provides **lightweight string views** without copying data.
	 * @li Works seamlessly with [BasicString](#Ares::BasicString) and [String](#Ares::String).
	 * @li Supports **fast substring operations**.
	 * @li Ideal for passing strings efficiently in APIs.
	 * 
	 * @note Unlike [String](#Ares::String), this does not manage memory. Ensure the referenced string remains valid.
	 * 
	 * @ingroup aliases
	 */
	using StringView = eastl::basic_string_view<char>;

}