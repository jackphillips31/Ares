/**
 * @file Core.h
 * @brief Defines platform detection macros and utility functions.
 * 
 * @details This file is responsible for detecting the target platform at compile-time
 * using predefined macros and defining appropriate platform-specific macros.
 * It also provides some utility macros for assertions, event binding, and
 * smart pointer creation.
 * 
 * The platform detection section defines macros for Windows, macOS, iOS,
 * Android, and Linux. Depending on the platform, different macros are defined
 * to ensure compatibility and proper platform behavior. Additionally, there are
 * assertions and event binding macros that are conditionally enabled based on
 * the build configuration.
 */
#pragma once
#include <memory>

/* Platform detection using predefined macros */
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/**
		 * @def AR_PLATFORM_WINDOWS
		 * @brief Defined for Windows platforms (x64 only).
		 * 
		 * @details This macro is defined if the target platform is WIndows (x64).
		 */
		#define AR_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/**
	 * TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MacOS
	 * and not some other Apple platform.
	 */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_PHONE == 1
		/**
		 * @def AR_PLATFORM_IOS
		 * @brief Defined for iOS platforms.
		 * 
		 * @details This macro is defined for iOS platforms, although iOS is currently unsupported.
		 */
		#define AR_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		/**
		 * @def AR_PLATFORM_MACOS
		 * @brief Defined for macOS platforms.
		 * 
		 * @details This macro is defined for macOS platforms, although macOS is currently unsupported.
		 */
		#define AR_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/*
 * We also have to check __ANDROID__ before __LINUX__
 * since android is based on the Linux kernel
 * it has __LINUX__ defined.
 */
#elif defined(__ANDROID__)
	/**
	 * @def AR_PLATFORM_ANDROID
	 * @brief Defined for Android platforms.
	 * 
	 * @details This macro is defined for Android platforms, although Android is currently unsupported.
	 */
	#define AR_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__LINUX__)
	/**
	 * @def AR_PLATFORM_LINUX
	 * @brief Defined for Linux platforms.
	 * 
	 * @details This macro is defined for Linux platforms, although Linux is currently unsupported.
	 */
	#define AR_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif
/* End of platform detection */

/**
 * @def AR_DEBUG
 * @brief Defined if the build is in debug mode.
 * 
 * @details This macro is used to enable or disable certain debug-specific features, such as
 * assertions and profiling.
 */
#ifdef AR_DEBUG

	/**
	 * @def AR_ENABLE_ASSERTS
	 * @brief Enables assertions in debug mode.
	 * 
	 * @details This macro is defined when assertions should be enabled in debug builds.
	 */
	#define AR_ENABLE_ASSERTS

	/**
	 * @def AR_PROFILE
	 * @brief Enables profiling in debug mode (if set to 1).
	 * 
	 * @details This macro is defined and set to one when profiling should be enabled in debug
	 * builds, although this is currently unsupported.
	 */
	#define AR_PROFILE 0
#endif

#ifdef AR_ENABLE_ASSERTS
	/**
	 * @def AR_ASSERT
	 * @brief Performs a runtime assertion check in the client systems.
	 * 
	 * @details If the condition `x` is false, this macro triggers an error log and
	 * breaks into the debugger.
	 * 
	 * @param x The condition to check.
	 * @param ... The format arguments for the error message.
	 */
	#define AR_ASSERT(x, ...) { if(!(x)) { AR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	/**
	 * @def AR_CORE_ASSERT
	 * @brief Performs a runtime assertion check in the core systems.
	 * 
	 * @details If the condition `x` is false, this macro triggers an error log and
	 * breaks into the debugger.
	 * 
	 * @param x The condition to check
	 * @param ... The format arguments for the error message.
	 */
	#define AR_CORE_ASSERT(x, ...) { if(!(x)) { AR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AR_ASSERT(x, ...)
	#define AR_CORE_ASSERT(x, ...)
#endif

/**
 * @def BIT(x)
 * @brief Bit-shift macro for setting a bit.
 * 
 * @details This macro returns a value with the `x`th bit set to 1.
 * 
 * @param x The bit index to set.
 */
#define BIT(x) (1 << x)

/**
 * @def AR_BIND_EVENT_FN
 * @brief Binds an event handler function tot he current object.
 * 
 * @details This macro creates a `std::bind` to bind the specified event handler
 * function to the current object and passes the event as a parameter.
 * 
 * @param fn The event handler function.
 */
#define AR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/**
 * @def AR_STATIC_BIND_EVENT_FN
 * @brief binds a static event handler function.
 * 
 * @details This macro creates a `std::bind` to bind a static event handler function
 * to the current object and passes the event as a parameter.
 * 
 * @param fn The static event handler function.
 */
#define AR_STATIC_BIND_EVENT_FN(fn) std::bind(&fn, std::placeholders::_1)

/**
 * @def AR_BIND_ASSET_FN
 * @brief Binds an asset handler function to the current object.
 * 
 * @details This macro creates a `std::bind` to bind the specified asset handler function
 * to the current object and passes the asset as a parameter.
 * 
 * @param fn The asset handler function.
 */
#define AR_BIND_ASSET_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/**
 * @def AR_STATIC_BIND_ASSET_FN
 * @brief Binds a static asset handler function.
 * 
 * @details This macro creates a `std::bind` to bind a static asset handler function to
 * the current object and passes the asset as a parameter.
 */
#define AR_STATIC_BIND_ASSET_FN(fn) std::bind(&fn, std::placeholders::_1)

/**
 * @namespace Ares
 * @brief The main namespace for the Ares engine.
 * 
 * @details This namespace encompasses all components of the Ares engine.
 */
namespace Ares {

	/**
	 * @typedef Scope
	 * @brief A unique pointer alias for managing heap-allocated objects.
	 * 
	 * @details This alias simplifies the usage of `std::unique_ptr` within the engine.
	 */
	template<typename T>
	using Scope = std::unique_ptr<T>;

	/**
	 * @brief Creates a unique pointer to a new object.
	 * 
	 * @details This utility function creates a new object and returns a Scope (unique pointer)
	 * to it. It forwards the provided arguments to the constructor of the object.
	 * 
	 * @param args The arguments to be forwarded to the object's constructor.
	 * @return A Scope<T> pointing to the newly created object.
	 */
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	/**
	 * @typedef Ref
	 * @brief A shared pointer alias for managing shared ownership of objects.
	 * 
	 * @details This alias simplifies the usage of `std::shared_ptr` within the engine.
	 */
	template<typename T>
	using Ref = std::shared_ptr<T>;

	/**
	 * @brief Creates a shared pointer to a new object.
	 * 
	 * @details This utility function creates a new object and returns a Ref (shared pointer)
	 * to it. It forwards the provided arguments to the constructor of the object.
	 * 
	 * @param args The arguments to be forwarded to the object's constructor.
	 * @return A Ref<T> pointing to the newly created object.
	 */
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}