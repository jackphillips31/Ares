#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64 */
		#define AR_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	*  so we must check all of them (in this order)
	*  to ensure that we're running on MacOS
	*  and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_PHONE == 1
		#define AR_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define AR_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __LINUX__
*  since android is based on the linux kernel
*  it has __LINUX__ defined */
#elif defined(__ANDROID__)
	#define AR_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__LINUX__)
	#define AR_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif
// End of platform detection

#ifdef AR_DEBUG
#define AR_ENABLE_ASSERTS
#define AR_PROFILE 0
#endif

#ifdef AR_ENABLE_ASSERTS
	#define AR_ASSERT(x, ...) { if(!(x)) { AR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AR_CORE_ASSERT(x, ...) { if(!(x)) { AR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AR_ASSERT(x, ...)
	#define AR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define AR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define AR_STATIC_BIND_EVENT_FN(fn) std::bind(&fn, std::placeholders::_1)

#define AR_BIND_ASSET_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define AR_BIND_ASSET_STATIC_FN(fn) std::bind(&fn, std::placeholders::_1)

namespace Ares {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// Helper function to combine hash values
	//template<typename T>
	//void CombineHash(size_t& seed, const T& value)
	//{
	//	seed ^= std::hash<T>{}(value)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	//}

}