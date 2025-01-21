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
#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>

/******************************************************/
/*     Platform detection using predefined macros     */
/******************************************************/

/**
 * Taken from EABase/config/eaplatform.h
 */

/**
 * @def AR_PLATFORM_MOBILE
 * @brief Define for mobile platforms.
 * 
 * @details AR_PLATFORM_MOBILE is a peer to AR_PLATFORM_DESKTOP and AR_PLATFORM_CONSOLE.
 * Their definition and qualitative rather than quantitative, and refers to the
 * general (usually weaker) capabilities of the machine. Mobile devices have a similar
 * set of weaknesses that are useful to generally categorize. The primary motivation is to
 * avoid code that test for multiple mobile platforms on a line and needs to be updated every
 * time we get a new one. For example, mobile platforms tend to have weaker ARM processors,
 * don't have full multiple processor support, are hand-held, don't have mice (though may
 * have touch screens or basic cursor controls), have writable solid state permanent storage.
 * Production user code shouldn't have too many expectations about the meaning of this define.
 */

/**
 * @def AR_PLATFORM_DESKTOP
 * @brief Define for desktop platforms.
 * 
 * @details This is similar to AR_PLATFORM_MOBILE in its qualitative nature and refers to platforms
 * that are powerful. For example, they nearly always have virtual memory, mapped memory, hundreds of GB of
 * writable disk storage, TCP/IP network connections, mice, keyboards, 512+ MB of RAM, multiprocessing,
 * multiple display support. Production user code shouldn't have too many expectations about the meaning
 * of this define.
 */

/**
 * @def AR_PLATFORM_CONSOLE
 * @brief Define for console platforms.
 * 
 * @details This is similar to AR_PLATFORM_MOBILE in its qualitative nature and refers to platforms
 * that are consoles. This means platforms that are connected to TVs, are fairly powerful (especially graphics-
 * wise), are tightly controlled by vendors, tend not to have mapped memory, tend to have TCP/IP, don't
 * have multiple process support though they might have multiple CPUs, support TV output only. Production
 * user code shouldn't have too many expectations about the meaning of this define.
 */

/**
 * @def AR_PLATFORM_CYGWIN
 * @brief This is a pseudo-platform which will be defined along with AR_PLATFORM_LINUX when
 * using the Cygwin build environment
 */
#if defined(__CYGWIN__)
	#define AR_PLATFORM_CYGWIN
	#define AR_PLATFORM_DESKTOP
#endif

/**
 * @def AR_PLATFORM_MINGW
 * @brief This is a pseudo-platform which will be defined along with AR_PLATFORM WINDOWS when
 * using the MinGW Windows build environment.
 */
#if defined(__MINGW32__) || defined(__MINGW64__)
	#define AR_PLATFORM_MINGW
	#define AR_PLATFORM_DESKTOP
#endif


#if defined(AR_PLATFORM_PS4) || defined(__ORBIS__)
	/**
	 * @def AR_PLATFORM_PS4
	 * @brief Define for the PS4 platform.
	 * @details Orbis was Sony's code-name for the platform, which is now obsolete.
	 */
	#if defined(AR_PLATFORM_PS4)
		#undef AR_PLATFORM_PS4
	#endif
	#define AR_PLATFORM_PS4

	#define AR_PLATFORM_NAME "PS4"
	#define AR_PLATFORM_DESCRIPTION "PS4 on x64"
	#define AR_SYSTEM_LITTLE_ENDIAN
	#define AR_PLATFORM_CONSOLE
	#define AR_PLATFORM_SONY
	#define AR_PLATFORM_POSIX
	#define AR_PROCESSOR_X86_64
	#if defined(__GNUC__) || defined(__clang__)
		#define AR_ASM_STYLE_ATT
	#endif
#elif defined(AR_PLATFORM_XBOXONE) || defined(_DURANGO) || defined(_XBOX_ONE) || defined(_GAMING_XBOX)
	/**
	 * @def AR_PLATFORM_XBOX
	 * @brief Define for the XBox platform.
	 * @details Durango was Microsoft's code-name for the platform, which is now obsolete.
	 * Microsoft uses _DURANGO instead of some variation of _XBOX, though it's not natively
	 * defined by the compiler.
	 */
	#if defined(AR_PLATFORM_XBOX)
		#undef AR_PLATFORM_XBOX
	#endif

	#if !defined(_DURANGO)
		#define _DURANGO
	#endif
	#define AR_PLATFORM_NAME "XBox One"
	#define AR_PLATFORM_DESCRIPTION "XBox One on x64"
	#define AR_SYSTEM_LITTLE_ENDIAN
	#define AR_ASM_STYLE_INTEL
	#define AR_PLATFORM_CONSOLE
	#define AR_PLATFORM_MICROSOFT
	#define AR_PROCESSOR_X86_64

	/* WINAPI_FAMILY defines - mirrored from winapifamily.h */
	#define AR_WINAPI_FAMILY_APP			1000
	#define AR_WINAPI_FAMILY_DESKTOP_APP	1001
	#define AR_WINAPI_FAMILY_PHONE_APP		1002
	#define AR_WINAPI_FAMILY_TV_APP			1003
	#define AR_WINAPI_FAMILY_TV_TITLE		1004
	#define AR_WINAPI_FAMILY_GAMES			1006

	#if defined (WINAPI_FAMILY)
		#include <winapifamily.h>
		#if defined(WINAPI_FAMILY_TV_TITLE) && WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_TV_TITLE
		#elif defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on XBox.
	#endif
	
	/* Macro to determine if a partition is enabled. */
	#define AR_WINAPI_FAMILY_PARTITION(Partition)	(Partition)

	#if AR_WINAPI_FAMILY == AR_WINAPI_FAMILY_DESKTOP_APP
		#define AR_WINAPI_PARTITION_CORE		1
		#define AR_WINAPI_PARTITION_DESKTOP		1
		#define AR_WINAPI_PARTITION_APP			1
		#define AR_WINAPI_PARTITION_PC_APP		0
		#define AR_WINAPI_PARTITION_PHONE		0
		#define AR_WINAPI_PARTITION_TV_APP		0
		#define AR_WINAPI_PARTITION_TV_TITLE	0
		#define AR_WINAPI_PARTITION_GAMES		0
	#elif AR_WINAPI_FAMILY == AR_WINAPI_FAMILY_TV_TITLE
		#define AR_WINAPI_PARTITION_CORE		1
		#define AR_WINAPI_PARTITION_DESKTOP		0
		#define AR_WINAPI_PARTITION_APP			0
		#define AR_WINAPI_PARTITION_PC_APP		0
		#define AR_WIANPI_PARTITION_PHONE		0
		#define AR_WINAPI_PARTITION_TV_APP		0
		#define AR_WINAPI_PARTITION_TV_TITLE	1
		#define AR_WINAPI_PARTITION_GAMES		0
	#elif AR_WINAPI_FAMILY == AR_WINAPI_FAMILY_GAMES
		#define AR_WINAPI_PARTITION_CORE		1
		#define AR_WINAPI_PARTITION_DESKTOP		0
		#define AR_WINAPI_PARTITION_APP			0
		#define AR_WINAPI_PARTITION_PC_APP		0
		#define AR_WIANPI_PARTITION_PHONE		0
		#define AR_WINAPI_PARTITION_TV_APP		0
		#define AR_WINAPI_PARTITION_TV_TITLE	0
		#define AR_WINAPI_PARTITION_GAMES		1
	#else
		#error Unsupported WINAPI_FAMILY
	#endif

	#if AR_WINAPI_FAMILY_PARTITION(AR_WINAPI_PARTITION_GAMES)
		#define CS_UNDEFINED_STRING 			1
		#define CS_UNDEFINED_STRING 		1
	#endif

	#if AR_WINAPI_FAMILY_PARTITION(AR_WINAPI_PARTITION_TV_TITLE)
		#define AR_PLATFORM_XBOXONE_XDK 	1
	#endif

	#if AR_WINAPI_FAMILY_PARTITION(AR_WINAPI_PARTITION_TV_TITLE)
		#define AR_PLATFORM_XBOXONE_XDK 	1
	#endif
#elif defined(AR_PLATFORM_LRB) || defined(__LRB__) || (defined(__EDG__) && defined(__ICC) && defined(__x86_64__))
	#undef AR_PLATFORM_LRB
	#define AR_PLATFORM_LRB
	#define AR_PLATFORM_NAME "Larrabee"
	#define AR_PLATFORM_DESCRIPTION "Larrabee on LRB1"
	#define AR_PROCESSOR_X86_64
	#if defined(BYTE_ORDER) && (BYTE_ORDER == 4321)
		#define AR_SYSTEM_BIG_ENDIAN
	#else
		#define AR_SYSTEM_LITTLE_ENDIAN
	#endif
	#define AR_PROCESSOR_LRB
	#define AR_PROCESSOR_LRB1
	#define AR_ASM_STYLE_ATT
	#define AR_ASM_STYLE_INTEL
	#define AR_PLATFORM_DESKTOP
#elif defined(AR_PLATFORM_ANDROID) || defined(__ANDROID__)
	/* Android (Google phone OS) */
	#undef AR_PLATFORM_ANDROID
	#define AR_PLATFORM_ANDROID
	#define AR_PLATFORM_LINUX
	#define AR_PLATFORM_UNIX
	#define AR_PLATFORM_POSIX
	#define AR_PLATFORM_NAME "Android"
	#define AR_ASM_STYLE_ATT
	#if defined(__arm__)
		#define AR_ABI_ARM_LINUX	// a.k.a. "ARM eabi"
		#define AR_PROCESSOR_ARM32
		#define AR_PLATFORM_DESCRIPTION "Android on ARM"
	#elif defined(__aarch64__)
		#define AR_PROCESSOR_ARM64
		#define AR_PLATFORM_DESCRIPTION "Android on ARM64"
	#elif defined(__i386__)
		#define AR_PROCESSOR_X86
		#define AR_PLATFORM_DESCRIPTION "Android on x86"
	#elif defined(__x86_64)
		#define AR_PROCESSOR_X86_64
		#define AR_PLATFORM_DESCRIPTION "Android on x64"
	#else
		#error Unknown processor
	#endif
	#if !defined(AR_SYSTEM_BIG_ENDIAN) && !defined(AR_SYSTEM_LITTLE_ENDIAN)
		#define AR_SYSTEM_LITTLE_ENDIAN
	#endif
	#define AR_PLATFORM_MOBILE
#elif defined(AR_PLATFORM_SAMSUNG_TV)
	#undef AR_PLATFORM_SAMSUNG_TV
	#define AR_PLATFORM_SAMSUNG_TV
	#define AR_PLATFORM_LINUX
	#define AR_PLATFORM_UNIX
	#define AR_PLATFORM_POSIX
	#define AR_PLATFORM_NAME "SamsungTV"
	#define AR_PLATFORM_DESCRIPTION "Samsung SMART TV on ARM"
	#define AR_ASM_STYLE_ATT
	#define AR_PROCESSOR_ARM32
	#define AR_ABI_ARM_LINUX	// a.k.a. "ARM eabi"
	#define AR_PROCESSOR_ARM7
#elif defined(__APPLE__) && __APPLE__
	#include <TargetConditionals.h>

	/* Apple family of operating systems */
	#define AR_PLATFORM_APPLE
	#define AR_PLATFORM_POSIX
	
	/* iPhone */
	/* TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc. */
	#if defined(AR_PLATFORM_IPHONE) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
		#undef AR_PLATFORM_IPHONE
		#define AR_PLATFORM_IPHONE
		#define AR_PLATFORM_NAME "iPhone"
		#define AR_ASM_STYLE_ATT
		#define AR_POSIX_THREADS_AVAILABLE 1
	#if defined(__arm__)
		#define AR_ABI_ARM_APPLE
		#define AR_PROCESSOR_ARM32
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "iPhone on ARM"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define AR_ABI_ARM64_APPLE
		#define AR_PROCESSOR_ARM64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "iPohone on ARM64"
	#elif defined(__i386__)
		#define AR_PLATFORM_IPHONE_SIMULATOR
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "iPhone simulator on x86"
	#elif defined(__x86_64) || defined(__amd64)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "iPhone simulator on x64"
	#else
		#error Unknown processor
	#endif
	#define AR_PLATFORM_MOBILE
#elif defined(AR_PLATFORM_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
	/**
	 * @def AR_PLATFORM_OSX
	 * @brief Define for Macintosh OSX platforms.
	 * @details TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
	 * However, TARGET_OS_MAC is defined to be 1 in all cases.
	 * __i386__ and __intel__ are defined by the GCC compiler.
	 * __dest_os is defined by the Metrowerks compiler.
	 * __MACH__ is defined by the Metrowerks and GCC compilers.
	 * powerc and __powerc are defined by the Metrowerks and GCC compilers.
	 */
	#undef AR_PLATFORM_OSX
	#define AR_PLATFORM_OSX
	#define AR_PLATFORM_UNIX
	#define AR_PLATFORM_POSIX
	#define AR_PLATFORM_NAME "OSX"
	#if defined(__i386__) || defined(__intel__)
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "OSX on x86"
	#elif defined(__x86_64) || defined(__amd64)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "OSX on ARM"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define AR_ABI_ARM64_APPLE
		#define AR_PROCESSOR_ARM64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "OSX on ARM64"
	#elif defined(__POWERPC64__) || defined(__powerpc64__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_64
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
	#elif defined(__POWERPC__) || defined(__powerpc__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_32
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "OSX on PowerPC"
	#else
		#error Unknown processor
	#endif
	#if defined(__GNUC__)
		#define AR_ASM_STYLE_ATT
	#else
		#define AR_ASM_STYLE_MOTOROLA
	#endif
		#define AR_PLATFORM_DESKTOP
	#else
		#error Unknown Apple Platform
	#endif
#elif defined(AR_PLATFORM_LINUX) || (defined(__linux) || defined(__linux__))
	/**
	 * @def AR_PLATFORM_LINUX
	 * @brief Define for Linux platforms
	 * @details // __linux and __linux__ are defined by the GCC and Borland compiler.
	 * __i386__ and __intel__ are defined by the GCC compiler.
	 * __i386__ is defined by the Metrowerks compiler.
	 * _M_IX86 is defined by the Borland compiler.
	 * __sparc__ is defined by the GCC compiler.
	 * __powerpc__ is defined by the GCC compiler.
	 * __ARM_EABI__ is defined by GCC on an ARM v6l (Raspberry Pi 1)
	 * __ARM_ARCH_7A__ is defined by GCC on an ARM v7l (Raspberry Pi 2)
	 */
	#undef AR_PLATFORM_LINUX
	#define AR_PLATFORM_LINUX
	#define AR_PLATFORM_UNIX
	#define AR_PLATFORM_POSIX
	#define AR_PLATFORM_NAME "Linux"
	#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Linux on x86"
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
		#define AR_ABI_ARM_LINUX
		#define AR_PROCESSOR_ARM32
		#define AR_PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define AR_PROCESSOR_ARM64
		#define AR_PLATFORM_DESCRIPTION "Linux on ARM64"
	#elif defined(__x86_64__)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Linux on x64"
	#elif defined(__powerpc64__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_64
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
	#elif defined(__powerpc__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_32
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Linux on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define AR_ASM_STYLE_ATT
	#endif
	#define AR_PLATFORM_DESKTOP
#elif defined(AR_PLATFORM_BSD) || (defined(__BSD__) || defined(__FreeBSD__))
	#undef  AR_PLATFORM_BSD
	#define AR_PLATFORM_BSD
	#define AR_PLATFORM_UNIX
	#define AR_PLATFORM_POSIX				// BSD's posix compliance is not identical to Linux's
	#define AR_PLATFORM_NAME "BSD Unix"
	#if defined(__i386__) || defined(__intel__)
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "BSD on x86"
	#elif defined(__x86_64__)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "BSD on x64"
	#elif defined(__powerpc64__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_64
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "BSD on PowerPC 64"
	#elif defined(__powerpc__)
		#define AR_PROCESSOR_POWERPC
		#define AR_PROCESSOR_POWERPC_32
		#define AR_SYSTEM_BIG_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "BSD on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if !defined(AR_PLATFORM_FREEBSD) && defined(__FreeBSD__)
		#define AR_PLATFORM_FREEBSD		// This is a variation of BSD.
	#endif
	#if defined(__GNUC__)
		#define AR_ASM_STYLE_ATT
	#endif
	#define AR_PLATFORM_DESKTOP
#elif defined(AR_PLATFORM_WINDOWS_PHONE)
	#undef AR_PLATFORM_WINDOWS_PHONE
	#define AR_PLATFORM_WINDOWS_PHONE
	#define AR_PLATFORM_NAME "Windows Phone"
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows Phone on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows Phone on X86"
	#elif defined(_M_ARM)
		#define AR_ABI_ARM_WINCE
		#define AR_PROCESSOR_ARM32
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows Phone on ARM"
	#else //Possibly other Windows Phone variants
		#error Unknown processor
		#error Unknown endianness
	#endif
	#define AR_PLATFORM_MICROSOFT
	
	// WINAPI_FAMILY defines - mirrored from winapifamily.h
	#define AR_WINAPI_FAMILY_APP			1
	#define AR_WINAPI_FAMILY_DESKTOP_APP	2
	#define AR_WINAPI_FAMILY_PHONE_APP		3
	
	#if defined(WINAPI_FAMILY)
		#include <winapifamily.h>
		#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_PHONE_APP
		#else
			#error Unsupported WINAPI_FAMILY for Windows Phone
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on Windows Phone.
	#endif
	
	// Macro to determine if a partition is enabled.
	#define AR_WINAPI_FAMILY_PARTITION(Partition)	(Partition)
	
	// Enable the appropriate partitions for the current family
	#if AR_WINAPI_FAMILY == AR_WINAPI_FAMILY_PHONE_APP
		#define AR_WINAPI_PARTITION_CORE
		#define AR_WINAPI_PARTITION_PHONE
		#define AR_WINAPI_PARTITION_APP
	#else
		#error Unsupported WINAPI_FAMILY for Windows Phone
	#endif
#elif (defined(AR_PLATFORM_WINDOW) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))) && !defined(CS_UNDEFINED_STRING)
	#undef AR_PLATFORM_WINDOWS
	#define AR_PLATFORM_WINDOWS
	#define AR_PLATFORM_NAME "Windows"
	#ifdef _WIN64	// VC++ defines both _WIN32 and _WIN64 when compiling for Win64
		#define AR_PLATFORM_WIN64
	#else
		#define AR_PLATFORM_WIN32
	#endif
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define AR_PROCESSOR_X86_64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define AR_PROCESSOR_X86
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows on x86"
	#elif defined(_M_IA64) || defined(_IA64_)
		#define AR_PROCESSOR_IA64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows on IA-64"
	#elif defined(_M_ARM)
		#define AR_ABI_ARM_WINCE
		#define AR_PROCESSOR_ARM32
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows on ARM"
	#elif defined(_M_ARM64)
		#define AR_PROCESSOR_ARM64
		#define AR_SYSTEM_LITTLE_ENDIAN
		#define AR_PLATFORM_DESCRIPTION "Windows on ARM64"
	#else // Possibly other Windows CE variants
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define AR_ASM_STYLE_ATT
	#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
		#define AR_ASM_STYLE_INTEL
	#endif
	#define AR_PLATFORM_DESKTOP
	#define AR_PLATFORM_MICROSOFT
	
	#if defined(_KERNEL_MODE)
		#define AR_PLATFORM_WINDOWS_KERNEL
	#endif
	
	/* WINAPI_FAMILY defines to support Windows 8 Metro Apps - mirroring winapifamily.h in the Windows 8 SDK */
	#define AR_WINAPI_FAMILY_APP			1000
	#define AR_WINAPI_FAMILY_DESKTOP_APP	1001
	#define AR_WINAPI_FAMILY_GAMES			1006
	
	#if defined(WINAPI_FAMILY)
		#if defined(_MSC_VER)
			#pragma warning(push, 0)
		#endif
		#include <winapifamily.h>
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		#if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_GAMES
		#else
			#error "Unsupported WINAPI_FAMILY"
		#endif
	#else
		#define AR_WINAPI_FAMILY AR_WINAPI_FAMILY_DESKTOP_APP
	#endif
	
	#define AR_WINAPI_PARTITION_DESKTOP
	#define AR_WINAPI_PARTITION_APP
	#define AR_WINAPI_PARTITION_GAMES (AR_WINAPI_FAMILY == AR_WINAPI_FAMILY_GAMES)
	
	#define AR_WINAPI_FAMILY_PARTITION(Partition)	(Partition)

	/**
	 * @def AR_PLATFORM_WINRT
	 * @brief Define for Windows subset used for tablets and the "Metro" (restricted) Windows user interface.
	 * @details WinRT doesn't doesn't have access to the Windows "desktop" API, but WinRT can nevertheless
	 * run on desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not
	 * included in it due to it being only a part of the API.
	 */
	#if defined(__cplusplus_winrt)
		#define AR_PLATFORM_WINRT
	#endif
#else
	/**
	 * @brief Sun (Solaris)
	 * @details __SUNPRO_CC is defined by the Sun compiler.
	 * __sun is defined by the GCC compiler.
	 * __i386 is defined by the Sun And GCC compilers.
	 * __sparc is defined by the Sun and GCC compilers.
	 */
	#error Unknown platform
	#error Unknown processor
	#error Unknown endianness
#endif

#ifndef AR_PROCESSOR_ARM
	#if defined(AR_PROCESSOR_ARM32) || defined(AR_PROCESSOR_ARM64) || defined(AR_PROCESSOR_ARM7)
		#define AR_PROCESSOR_ARM
	#endif
#endif
/******************************************************/
/*              End of platform detection             */
/******************************************************/

/******************************************************/
/*   Pointer size detection using predefined macros   */
/******************************************************/
/**
 * @def AR_PLATFORM_PTR_SIZE
 * @brief Defines the size of a pointer (in bytes) for the current platform.
 * 
 * @details The value of AR_PLATFORM_PTR_SIZE is set based on the target platform
 * at compile-time. It is typically 4 bytes for 32-bit platforms and 8 bytes for
 * 64-bit platforms. This macro is used to ensure proper alignment and platform
 * independence in pointer-related operations.
 */
#ifndef AR_PLATFORM_PTR_SIZE
	#if defined(__WORDSIZE) // Defined by some variations of GCC.
		#define AR_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
	#elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
		#define AR_PLATFORM_PTR_SIZE 8
	#elif defined(__CC_ARM) && (__sizeof_ptr == 8)
		#define AR_PLATFORM_PTR_SIZE 8
	#else
		#define AR_PLATFORM_PTR_SIZE 4
	#endif
#endif
/******************************************************/
/*            End of pointer size detection           */
/******************************************************/

/******************************************************/
/*                Word Size Detection                 */
/******************************************************/
/**
 * @def AR_PLATFORM_WORD_SIZE
 * @brief Defines the size of a machine word.
 * @details This will be the same as the size of registers on the machine 
 * but not necessarily the same as the size of pointers on the machine.
 * A number of 64 bit platforms have 64 bit registers but 32 bit pointers.
 */
#ifndef AR_PLATFORM_WORD_SIZE
	#define AR_PLATFORM_WORD_SIZE AR_PLATFORM_PTR_SIZE
#endif
/******************************************************/
/*              End Word Size Detection               */
/******************************************************/

/******************************************************/
/*         Minimum Alloc Alignment Detection          */
/******************************************************/
/**
 * @def AR_PLATFORM_MIN_MALLOC_ALIGNMENT
 * @brief Defines the minimal alignment that the platform's malloc implementation
 * will return.
 * @details This should be used when writing custom allocators to ensure that the
 * alignment matches that of malloc.
 */
#ifndef AR_PLATFORM_MIN_MALLOC_ALIGNMENT
	#if defined(AR_PLATFORM_APPLE)
		#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT 16
	#elif defined(AR_PLATFORM_ANDROID) && defined(AR_PROCESSOR_ARM)
		#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT 8
	#elif defined(AR_PLATFORM_ANDROID) && defined(AR_PROCESSOR_X86_64)
		#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT 8
	#else
		#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT (AR_PLATFORM_PTR_SIZE * 2)
	#endif
#endif
/******************************************************/
/*       End Minimum Alloc Alignment Detection        */
/******************************************************/

/******************************************************/
/*             Misaligned Support Level               */
/******************************************************/
/**
 * @def AR_MISALIGNED_SUPPORT_LEVEL
 * @brief Specifies if the processor can read and write built-in types that aren't
 * naturally aligned.
 *  0 - not supported. Likely causes an exception.
 *  1 - supported but slow.
 *  2 - supported and fast.
 */
#ifndef AR_MISALIGNED_SUPPORT_LEVEL
	#if defined(AR_PROCESSOR_X86_64)
		#define AR_MISALIGNED_SUPPORT_LEVEL 2
	#else
		#define AR_MISALIGNED_SUPPORT_LEVEL 0
	#endif
#endif
/******************************************************/
/*           End Misaligned Support Level             */
/******************************************************/

/******************************************************/
/*                  Cache Line Size                   */
/******************************************************/
/**
 * @def AR_CACHE_LINE_SIZE
 * @brief Specifies the cache line size broken down by the compile target.
 * @details This is expected best guess values for the targets that we can
 * make at compilation time.
 */
#ifndef AR_CACHE_LINE_SIZE
	#if defined(AR_PROCESSOR_X86)
		#define AR_CACHE_LINE_SIZE 32		// This is the minimum possible value
	#elif defined(AR_PROCESSOR_X86_64)
		#define AR_CACHE_LINE_SIZE 64		// This is the minimum possible value
	#elif defined(AR_PROCESSOR_ARM32)
		#define AR_CACHE_LINE_SIZE 32		// This varies between implementations and is usually 32 or 64
	#elif defined(AR_PROCESSOR_ARM64)
		#define AR_CACHE_LINE_SIZE 64		// Cache line Cortex-A8 (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
	#elif defined(AR_PLATFORM_WORD_SIZE == 4)
		#define AR_CACHE_LINE_SIZE 32		// This is the minimum possible value
	#else
		#define AR_CACHE_LINE_SIZE 64		// This is the minimum possible value
	#endif
#endif
/******************************************************/
/*                End Cache Line Size                 */
/******************************************************/

/******************************************************/
/*                 Ares Debug Defines                 */
/******************************************************/
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

	/**
	 * @def EASTL_DEBUG
	 * @brief Enabled debug features of EA Standard Library (if set to 1)
	 * 
	 * @details This macro is defined and set to one when debugging.
	 */
	#define EASTL_DEBUG 1
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
/******************************************************/
/*              End Ares Debug Defines                */
/******************************************************/

/******************************************************/
/*                        Ares                        */
/******************************************************/
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
 * @brief Binds a static event handler function.
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
	 * @details This alias simplifies the usage of `eastl::unique_ptr` within the engine.
	 */
	template<typename T>
	using Scope = eastl::unique_ptr<T>;

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
		return eastl::make_unique<T>(std::forward<Args>(args)...);
	}

	/**
	 * @typedef Ref
	 * @brief A shared pointer alias for managing shared ownership of objects.
	 * 
	 * @details This alias simplifies the usage of `eastl::shared_ptr` within the engine.
	 */
	template<typename T>
	using Ref = eastl::shared_ptr<T>;

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
		return eastl::make_shared<T>(std::forward<Args>(args)...);
	}

}
/******************************************************/
/*                      End Ares                      */
/******************************************************/