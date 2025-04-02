@page macros Macros
@tableofcontents
@section macros_intro Introduction
Welcome to the **Macro Documentation** page. This section provides detailed information about the various macros used across the project. Each section will contain a group of related macros, organized by functionality.

@subsection macros_platform_defines Platform Macros
Macros used to identify the target platform during compilation.<br>
@subpage platform_defines

@subsection macros_processor_defines Processor Architecture Macros
Macros used to identify the target processor architecture.<br>
@subpage processor_defines

@subsection macros_endian_defines Endianness Macros
Macros used to determine the endianness of the target system.<br>
@subpage endianness_defines

@subsection macros_compiler_defines Compiler and Assembly Macros
Macros for compiler and assembly-level operations.<br>
@subpage compiler_defines

@subsection macros_system_defines System Configuration Macros
Macros for system-level configuration and capabilities.<br>
@subpage system_defines

@subsection macros_debug_defines Debug and Assert Macros
Macros for debugging, assertions, and error checking.<br>
@subpage debug_defines

@subsection macros_utility_defines Utility Macros
General-purpose macros for simplifying common tasks.<br>
@subpage utility_defines

@page platform_defines Platform Macros
@tableofcontents
@brief Macros used to identify the target platform during compilation.
@details This group includes a set of macros that are automatically defined or can be manually specified to detect the platform (e.g., Windows, Linux, macOS, etc.) the code is being compiled for. These macros are typically set in the build system or by the compiler and can be used for platform-specific logic and conditional compilation.<br><br> <b>A few defintions were copied from `EABase/config/eaplatform.h`.</b>

@section list_platform_defines List of Macros
@ref AR_PLATFORM_MOBILE <br>
@ref AR_PLATFORM_DESKTOP <br>
@ref AR_PLATFORM_CONSOLE <br>
@ref AR_PLATFORM_WINDOWS <br>
@ref AR_PLATFORM_MINGW <br>
@ref AR_PLATFORM_WINRT <br>
@ref AR_PLATFORM_LINUX <br>
@ref AR_PLATFORM_CYGWIN <br>
@ref AR_PLATFORM_OSX <br>
@ref AR_PLATFORM_IPHONE <br>
@ref AR_PLATFORM_ANDROID <br>
@ref AR_PLATFORM_PS4 <br>
@ref AR_PLATFORM_XBOXONE

@subsection AR_PLATFORM_MOBILE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_MOBILE">◆&nbsp;</a></span>AR_PLATFORM_MOBILE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_MOBILE&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for mobile platforms (iOS, Android).</b><br><br> @ref AR_PLATFORM_MOBILE is a peer to @ref AR_PLATFORM_DESKTOP and @ref AR_PLATFORM_CONSOLE. Their definition and qualitative rather than quantitative, and refers to the general (usually weaker) capabilities of the machine. Mobile devices have a similar set of weaknesses that are useful to generally categorize. The primary motivation is to avoid code that test for multiple mobile platforms on a line and needs to be updated every time we get a new one. For example, mobile platforms tend to have weaker ARM processors, don't have full multiple processor support, are hand-held, don't have mice (though may have touch screens or basic cursor controls), have writable solid state permanent storage. Production user code shouldn't have too many expectations about the meaning of this define.</div>
</div>

@subsection AR_PLATFORM_DESKTOP
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_DESKTOP">◆&nbsp;</a></span>AR_PLATFORM_DESKTOP</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_DESKTOP&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for desktop platforms (Windows, macOS, Linux).</b><br><br> This is similar to @ref AR_PLATFORM_MOBILE in its qualitative nature and refers to platforms that are powerful. For example, they nearly always have virtual memory, mapped memory, hundreds of GB of writable disk storage, TCP/IP network connections, mice, keyboards, 512+ MB of RAM, multiprocessing, multiple display support. Production user code shouldn't have too many expectations about the meaning of this define.</div>
</div>

@subsection AR_PLATFORM_CONSOLE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_CONSOLE">◆&nbsp;</a></span>AR_PLATFORM_CONSOLE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_CONSOLE&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for console platforms (PlayStation, XBox).</b><br><br> This is similar to @ref AR_PLATFORM_MOBILE in its qualitative nature and refers to platforms that are consoles. This means platforms that are connected to TVs, are fairly powerful (especially graphics-wise), are tightly controlled by vendors, tend not to have mapped memory, tend to have TCP/IP, don't have multiple process support though they might have multiple CPUs, support TV output only. Production user code shouldn't have too many expectations about the meaning of this define.</div>
</div>

@subsection AR_PLATFORM_WINDOWS
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_WINDOWS">◆&nbsp;</a></span>AR_PLATFORM_WINDOWS</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_WINDOWS&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for Windows platforms.</b><br><br> This macro is defined during compilation to indicate that the target platform is a Windows operating system. It is used for conditional compilation of Windows-specific code, such as system calls, library usage, or platform-dependent optimizations.</div>
</div>

@subsection AR_PLATFORM_MINGW
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_MINGW">◆&nbsp;</a></span>AR_PLATFORM_MINGW</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_MINGW&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>This is a pseudo-platform which will be defined with a value of <code>1</code> along with @ref AR_PLATFORM_WINDOWS when using the MinGW Windows build environment.</b><br><br> This macro is defined during compilation to indicate that the target platform is a Windows operating system. It is used for conditional compilation of Windows-specific code, such as system calls, library usage, or platform-dependent optimizations.</div>
</div>

@subsection AR_PLATFORM_WINRT
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_WINRT">◆&nbsp;</a></span>AR_PLATFORM_WINRT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_WINRT&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined for Windows subset with a value of <code>1</code> used for tablets and the "Metro" (restricted) Windows user interface.</b><br><br> WinRT doesn't doesn't have access to the Windows "desktop" API, but WinRT can nevertheless run on desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not included in it due to it being only a part of the API.</div>
</div>

@subsection AR_PLATFORM_LINUX
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_LINUX">◆&nbsp;</a></span>AR_PLATFORM_LINUX</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_LINUX&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for Linux platforms.</b><br><br> <ul><li>***__linux*** and ***__linux__*** are defined by the GCC and Borland compiler.</li><li>***__i386__*** and ***__intel__*** are defined by the GCC compiler.</li><li>***__i386__*** is defined by the Metrowerks compiler.</li><li>***_M_IX86*** is defined by the Borland compiler.</li><li>***__sparc__*** is defined by the GCC compiler.</li><li>***__powerpc__*** is defined by the GCC compiler.
</li><li>***__ARM_EABI__*** is defined by GCC on an ARM v6l (Raspberry Pi 1).</li><li>***__ARM_ARCH_7A__*** is defined by GCC on an ARM v7l (Raspberry Pi 2)</li></ul></div>
</div>

@subsection AR_PLATFORM_CYGWIN
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_CYGWIN">◆&nbsp;</a></span>AR_PLATFORM_CYGWIN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_CYGWIN&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>This is a pseudo-platform which will be defined with a value of 1 along with @ref AR_PLATFORM_LINUX when using the Cygwin build environment.</b><br><br> This macro is defined when compiling in the Cygwin environment, which provides a Linux-like API on top of the Windows operating system. It is typically used for building software that should be compatible with both Linux and Cygwin environments, enabling platform-specific conditional compilation. The macro is defined alongside @ref AR_PLATFORM_LINUX to differentiate between native Linux and Cygwin-specific code paths.</div>
</div>

@subsection AR_PLATFORM_OSX
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_OSX">◆&nbsp;</a></span>AR_PLATFORM_OSX</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_OSX&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for macOS.</b><br><br> <ul><li>***TARGET_OS_MAC*** is defined by the Metrowerks and older AppleC compilers.However, **TARGET_OS_MAC** is defined to be 1 in all cases.</li><li>***__i386__*** and ***__intel__*** are defined by the GCC compiler.</li><li>***__dest_os*** is defined by the Metrowerks compiler.</li><li>***__MACH__*** is defined by the Metrowerks and GCC compilers.</li><li>***powerc*** and ***__powerc*** are defined by the Metrowerks and GCC compilers.</li></ul></div>
</div>

@subsection AR_PLATFORM_IPHONE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_IPHONE">◆&nbsp;</a></span>AR_PLATFORM_IPHONE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_IPHONE&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for iOS devices.</b><br><br> This macro is defined during compilation when targeting Apple's iOS devices, such as iPhones or iPads. It is commonly used for conditional compilation of iOS-specific features, frameworks, or optimizations.</div>
</div>

@subsection AR_PLATFORM_ANDROID
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_ANDROID">◆&nbsp;</a></span>AR_PLATFORM_ANDROID</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_ANDROID&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for Android devices.</b><br><br> This macro is defined during compilation to indicate that the target platform is an Android device. It is used for conditional compilation of Android-specific code, including platform libraries, system calls, and features unique to Android.</div>
</div>

@subsection AR_PLATFORM_PS4
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_PS4">◆&nbsp;</a></span>AR_PLATFORM_PS4</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_PS4&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for PlayStation platforms.</b><br><br> This macro is defined when targeting the PlayStation 4 (PS4) platform. The PS4, developed by Sony, is a home video game console based on an x86-64 architecture. While the platform was code-named "Orbis" during its development, that name is now obsolete. This macro is used for enabling PS4-specific optimizations, system calls, and handling platform-specific APIs or features when compiling for PlayStation 4.<br><br> The PS4 has its own set of SDKs (Software Development Kits), libraries, and unique hardware capabilities that are specific to the console. This macro ensures that any code compiled for PS4 uses these APIs and features, including the graphics engine (based on AMD's GCN architecture), memory optimizations, and specific hardware functions like the DualShock 4 controller support or the PlayStation Network integration.</div>
</div>

@subsection AR_PLATFORM_XBOXONE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_XBOXONE">◆&nbsp;</a></span>AR_PLATFORM_XBOXONE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_XBOXONE&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when building for XBox One platforms.</b><br><br> This macro is defined when targeting the Xbox One platform. The Xbox One is a home video game console developed by Microsoft, and was originally codenamed "Durango" during its development phase. While the "Durango" code-name is now obsolete, this macro helps distinguish Xbox One-specific code paths.<br><br> Microsoft typically uses the `_DURANGO` identifier to represent the Xbox One platform in its SDKs and development tools, although this macro is not natively defined by the compiler itself. This macro is useful for enabling platform-specific features, system calls, and optimizations that are unique to the Xbox One, such as interaction with the Xbox Live service, controller support, or hardware-specific optimizations.<br><br> The Xbox One is based on an x86-64 architecture, and like other gaming consoles, it has a specific set of SDKs and APIs designed for its hardware. Using @ref AR_PLATFORM_XBOXONE, developers can conditionally compile Xbox One-specific features, ensuring compatibility with the console's unique system architecture and capabilities, such as the integration with Microsoft’s cloud services and the Xbox Store.</div>
</div>

@page processor_defines Processor Architecture Macros
@tableofcontents
@brief Macros used to identify the target processor architecture.
@details This group contains macros that help determine the processor architecture being targeted by the compiler. These macros enable conditional compilation based on whether the code is compiled for x86, x64, ARM, or other architectures. They are useful for optimizing code, ensuring compatibility, and handling architecture-specific features or constraints.<br><br>
<b>A few defintions were copied from `EABase/config/eaplatform.h`.</b>

@section list_processor_defines List of Macros
@ref AR_PROCESSOR_X86 <br>
@ref AR_PROCESSOR_X86_64 <br>
@ref AR_PROCESSOR_ARM32 <br>
@ref AR_PROCESSOR_ARM64 <br>
@ref AR_PROCESSOR_POWERPC <br>
@ref AR_PROCESSOR_POWERPC_32 <br>
@ref AR_PROCESSOR_POWERPC_64

@subsection AR_PROCESSOR_X86
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_X86">◆&nbsp;</a></span>AR_PROCESSOR_X86</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_X86&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for x86 32-bit processors.</b><br><br> This macro is defined when the target architecture is a 32-bit x86 processor. It is used to compile code specific to 32-bit x86 architectures, such as optimizations or assembly instructions tailored for this processor family. This macro helps in differentiating between 32-bit x86 and other processor types like x86_64 or ARM.</div>
</div>

@subsection AR_PROCESSOR_X86_64
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_X86_64">◆&nbsp;</a></span>AR_PROCESSOR_X86_64</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_X86_64&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for x86 64-bit processors.</b><br><br> This macro is defined when the target architecture is a 64-bit x86 processor. It is used for enabling optimizations and features specific to x86-64 architectures. This macro is particularly useful for distinguishing between 64-bit and 32-bit versions of x86 processors, enabling the compilation of 64-bit specific code or handling memory addressing for 64-bit systems.</div>
</div>

@subsection AR_PROCESSOR_ARM32
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_ARM32">◆&nbsp;</a></span>AR_PROCESSOR_ARM32</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_ARM32&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for 32-bit ARM processors.</b><br><br> This macro is defined when targeting a 32-bit ARM processor. ARM processors are commonly used in mobile devices, embedded systems, and some desktops. This macro is essential for conditional compilation of ARM-specific optimizations or instructions in 32-bit mode, ensuring compatibility with ARM-based systems.</div>
</div>

@subsection AR_PROCESSOR_ARM64
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_ARM64">◆&nbsp;</a></span>AR_PROCESSOR_ARM64</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_ARM64&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for 64-bit ARM processors.</b><br><br> This macro is defined when targeting a 64-bit ARM processor, such as those found in newer mobile devices, ARM-based servers, and certain desktop platforms. It enables the inclusion of 64-bit ARM-specific optimizations or code paths, distinguishing between ARM 32-bit and ARM 64-bit architectures.</div>
</div>

@subsection AR_PROCESSOR_POWERPC
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_POWERPC">◆&nbsp;</a></span>AR_PROCESSOR_POWERPC</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_POWERPC&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for PowerPC processors.</b><br><br> This macro is defined when targeting PowerPC processors. PowerPC was widely used in older Apple computers and embedded systems. Although less common now, this macro is still used in systems where PowerPC architecture is relevant. It helps to enable architecture-specific optimizations or workarounds for PowerPC systems.</div>
</div>

@subsection AR_PROCESSOR_POWERPC_32
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_POWERPC_32">◆&nbsp;</a></span>AR_PROCESSOR_POWERPC_32</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_POWERPC_32&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for 32-bit PowerPC processors.</b><br><br> This macro is defined when targeting 32-bit PowerPC processors. PowerPC was widely used in older Apple computers and embedded systems. Although less common now, this macro is still used in systems where PowerPC architecture is relevant. It helps to enable architecture-specific optimizations or workarounds for PowerPC systems.</div>
</div>

@subsection AR_PROCESSOR_POWERPC_64
<h2 class="memtitle"><span class="permalink"><a href="#AR_PROCESSOR_POWERPC_64">◆&nbsp;</a></span>AR_PROCESSOR_POWERPC_64</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PROCESSOR_POWERPC_64&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for 64-bit PowerPC processors.</b><br><br> This macro is defined when targeting 64-bit PowerPC processors. PowerPC was widely used in older Apple computers and embedded systems. Although less common now, this macro is still used in systems where PowerPC architecture is relevant. It helps to enable architecture-specific optimizations or workarounds for PowerPC systems.</div>
</div>

@page endianness_defines Endianness Macros
@tableofcontents
@brief Macros used to determine the endianness of the target system.
@details This group includes macros that indicate the byte order (endianness) of the target system. Endianness defines how multi-byte data is stored in memory, either as least significant byte first (little-endian) or most significant byte first (big-endian). These macros are essential for writing portable code that needs to interpret or manipulate raw binary data or work with network protocols, file formats, or hardware with specific endianness requirements.<br><br>
<b>A few defintions were copied from `EABase/config/eaplatform.h`.</b>

@section list_endianness_defines List of Macros
@ref AR_SYSTEM_LITTLE_ENDIAN <br>
@ref AR_SYSTEM_BIG_ENDIAN

@subsection AR_SYSTEM_LITTLE_ENDIAN
<h2 class="memtitle"><span class="permalink"><a href="#AR_SYSTEM_LITTLE_ENDIAN">◆&nbsp;</a></span>AR_SYSTEM_LITTLE_ENDIAN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_SYSTEM_LITTLE_ENDIAN&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for little-endian systems.</b></div>
</div>

@subsection AR_SYSTEM_BIG_ENDIAN
<h2 class="memtitle"><span class="permalink"><a href="#AR_SYSTEM_BIG_ENDIAN">◆&nbsp;</a></span>AR_SYSTEM_BIG_ENDIAN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_SYSTEM_BIG_ENDIAN&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> for big-endian systems.</b></div>
</div>

@page compiler_defines Compiler and Assembly Macros
@tableofcontents
@brief Macros for compiler and assembly-level operations.
@details This group contains macros used to detect the compiler, perform compiler-specific optimizations, and interact with assembly language. These macros enable conditional compilation based on the compiler being used (e.g., GCC, MSVC, Clang) and provide functionality for inline assembly, alignment, and other low-level tasks.<br><br>Common use cases include ensuring portability across compilers, enabling or disabling specific compiler features, and handling architecture-specific assembly instructions.<br><br>
<b>A few defintions were copied from `EABase/config/eaplatform.h`.</b>

@section list_compiler_defines List of Macros
@ref AR_ASM_STYLE_ATT <br>
@ref AR_ASM_STYLE_INTEL

@subsection AR_ASM_STYLE_ATT
<h2 class="memtitle"><span class="permalink"><a href="#AR_ASM_STYLE_ATT">◆&nbsp;</a></span>AR_ASM_STYLE_ATT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_ASM_STYLE_ATT&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when using AT&T style assembly syntax.</b><br><br>This macro is defined when the project uses AT&T assembly syntax, which is a popular syntax format used in many Unix-like systems, including Linux. AT&T syntax is characterized by its operand ordering (source before destination) and specific conventions for register names, instructions, and immediate values.<br><br>AT&T style assembly is often used in the GNU assembler (GAS), which is part of the GNU Binutils. It is widely used in environments where the GNU toolchain is standard, especially in open-source projects targeting Unix-based platforms.<br><br>The use of @ref AR_ASM_STYLE_ATT allows the code to be conditionally compiled to follow AT&T-specific syntax conventions, ensuring compatibility with assemblers that adhere to this style.</div>
</div>

@subsection AR_ASM_STYLE_INTEL
<h2 class="memtitle"><span class="permalink"><a href="#AR_ASM_STYLE_INTEL">◆&nbsp;</a></span>AR_ASM_STYLE_INTEL</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_ASM_STYLE_INTEL&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> when using Intel style assembly syntax.</b><br><br>This macro is defined when the project uses Intel assembly syntax, which is commonly used in environments such as Microsoft Visual Studio and Intel's own assemblers. Intel syntax differs from AT&T syntax in its operand ordering, where the destination comes before the source. Additionally, it uses different conventions for registers, instructions, and immediate values.<br><br>Intel style assembly is often preferred in x86 development, especially on Windows platforms, and is widely supported in assemblers such as MASM (Microsoft Macro Assembler) and NASM (Netwide Assembler).<br><br>Defining @ref AR_ASM_STYLE_INTEL allows the code to follow Intel-specific syntax rules, making it compatible with Intel-syntax-based assemblers, ensuring proper formatting and correctness when compiling assembly code.</div>
</div>

@page system_defines System Configuration Macros
@tableofcontents
@brief Macros for system-level configuration and capabilities.
@details This group includes macros that provide information about the system's configuration and capabilities, such as available features, memory limits, and hardware-specific details. These macros are often used to tailor the application to the runtime environment, ensuring efficient and compatible execution.<br><br>Examples include macros for memory alignment, system-specific libraries, or features like threading support.<br><br>
<b>A few defintions were copied from `EABase/config/eaplatform.h`.</b>

@section list_system_defines List of Macros
@ref AR_PLATFORM_PTR_SIZE <br>
@ref AR_PLATFORM_WORD_SIZE <br>
@ref AR_PLATFORM_MIN_MALLOC_ALIGNMENT <br>
@ref AR_MISALIGNED_SUPPORT_LEVEL <br>
@ref AR_CACHE_LINE_SIZE

@subsection AR_PLATFORM_PTR_SIZE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_PTR_SIZE">◆&nbsp;</a></span>AR_PLATFORM_PTR_SIZE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_PTR_SIZE</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defines the size of a pointer (in bytes) for the current platform.</b><br><br>The value of @ref AR_PLATFORM_PTR_SIZE is set based on the target platform at compile-time. It is typically 4 bytes for 32-bit platforms and 8 bytes for64-bit platforms. This macro is used to ensure proper alignment and platformindependence in pointer-related operations.</div>
</div>

@subsection AR_PLATFORM_WORD_SIZE
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_WORD_SIZE">◆&nbsp;</a></span>AR_PLATFORM_WORD_SIZE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_WORD_SIZE</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defines the size (in bytes) of a machine word.</b><br><br>This will be the same as the size of registers on the machine but not necessarily the same as the size of pointers on the machine. A number of 64 bit platforms have 64 bit registers but 32 bit pointers.</div>
</div>

@subsection AR_PLATFORM_MIN_MALLOC_ALIGNMENT
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_MIN_MALLOC_ALIGNMENT">◆&nbsp;</a></span>AR_PLATFORM_MIN_MALLOC_ALIGNMENT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_PLATFORM_MIN_MALLOC_ALIGNMENT</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defines the minimal alignment (in bytes) that the platform's malloc implementation will return.</b><br><br>This should be used when writing custom allocators to ensure that the alignment matches that of malloc.</div>
</div>

@subsection AR_MISALIGNED_SUPPORT_LEVEL
<h2 class="memtitle"><span class="permalink"><a href="#AR_PLATFORM_MISALIGNED_SUPPORT_LEVEL">◆&nbsp;</a></span>AR_MISALIGNED_SUPPORT_LEVEL</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_MISALIGNED_SUPPORT_LEVEL</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Specifies if the processor can read and write built-in types that aren't naturally aligned.</b><br><br><ul><li><b>0</b> - Not supported. Likely causes an exception.</li><li><b>1</b> - Supported but slow.</li><li><b>2</b> - Supported and fast.</li></ul></div>
</div>

@subsection AR_CACHE_LINE_SIZE
<h2 class="memtitle"><span class="permalink"><a href="#AR_CACHE_LINE_SIZE">◆&nbsp;</a></span>AR_CACHE_LINE_SIZE</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_CACHE_LINE_SIZE</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Specifies the cache line size (in bytes) broken down by the compile target.</b><br><br>This is expected best guess values for the targets that we can make at compilation time.</div>
</div>

@page debug_defines Debug and Assert Macros
@tableofcontents
@brief Macros for debugging, assertions, and error checking.
@details This group contains macros designed to assist with debugging and error detection during development. Common features include assertion checks, debug logging, and diagnostic outputs. These macros help ensure code correctness and simplify the debugging process by providing clear runtime feedback on issues.<br><br>Typical macros in this group include conditional logging (e.g., only in debug builds) and assertions that trigger when certain conditions fail.

@section list_debug_defines List of Macros
@ref AR_BUILD_DEBUG <br>
@ref AR_ENABLE_ASSERTS <br>
@ref AR_ENABLE_PROFILING <br>
@ref AR_ASSERT <br>
@ref AR_CORE_ASSERT

@subsection AR_BUILD_DEBUG
<h2 class="memtitle"><span class="permalink"><a href="#AR_BUILD_DEBUG">◆&nbsp;</a></span>AR_BUILD_DEBUG</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_BUILD_DEBUG&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> if the build is in debug mode.</b><br><br>This macro is used to enable or disable certain debug-specific features, such as assertions and profiling.</div>
</div>

@subsection AR_ENABLE_ASSERTS
<h2 class="memtitle"><span class="permalink"><a href="#AR_ENABLE_ASSERTS">◆&nbsp;</a></span>AR_ENABLE_ASSERTS</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_ENABLE_ASSERTS&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> to enable assertions in debug mode.</b><br><br>This macro's value is set to <code>1</code> when @ref AR_BUILD_DEBUG is set.</div>
</div>

@subsection AR_ENABLE_PROFILING
<h2 class="memtitle"><span class="permalink"><a href="#AR_ENABLE_PROFILING">◆&nbsp;</a></span>AR_ENABLE_PROFILING</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_ENABLE_PROFILING&nbsp;&nbsp;&nbsp; 1</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc" style="padding-top: 1em; padding-bottom: 1em"><b>Defined with a value of <code>1</code> to enable profiling in debug mode.</b><br><br>This macro's value is set to <code>1</code> when profiling should be enabled in debug builds. <dl class="section warning"><dt>Warning</dt><dd>This is currently <b>unsupported</b>. </dd></dl></div>
</div>

@subsection AR_ASSERT
<h2 class="memtitle"><span class="permalink"><a href="#AR_ASSERT">◆&nbsp;</a></span>AR_ASSERT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_ASSERT</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>x</em></span>, ... </td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		{ if(!(x)) { AR_ERROR("Assertion Failed: {0}", VA_ARGS); __debugbreak(); } } 
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">If the condition <code>x</code> is false, this macro triggers an error log and breaks into the debugger.</div>
		<dl class="section note"><dt>Note</dt><dd>Only defined when <a class="el" href="debug_defines.html#AR_ENABLE_ASSERTS">AR_ENABLE_ASSERTS</a> is set.</dd></dl>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">x</td><td>The condition to check. </td></tr>
					<tr><td class="paramname">...</td><td>The format arguments for the error message. </td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_CORE_ASSERT
<h2 class="memtitle"><span class="permalink"><a href="#AR_CORE_ASSERT">◆&nbsp;</a></span>AR_CORE_ASSERT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_CORE_ASSERT</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>x</em></span>, ... </td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		{ if(!(x)) { AR_CORE_ERROR("Assertion Failed: {0}", VA_ARGS); __debugbreak(); } } 
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">If the condition <code>x</code> is false, this macro triggers an error log and breaks into the debugger.</div>
		<dl class="section note"><dt>Note</dt><dd>Only defined when <a class="el" href="debug_defines.html#AR_ENABLE_ASSERTS">AR_ENABLE_ASSERTS</a> is set.</dd></dl>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">x</td><td>The condition to check. </td></tr>
					<tr><td class="paramname">...</td><td>The format arguments for the error message. </td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@page utility_defines Utility Macros
@tableofcontents
@brief General-purpose macros for simplifying common tasks.
@details This group contains a collection of versatile and reusable macros that help simplify common programming tasks. These macros may include functionality for string manipulation, mathematical calculations, type checks, and other helper utilities.<br><br>They are designed to improve code readability and reduce redundancy by encapsulating frequently used patterns into concise macro definitions.

@section list_utility_defines List of Macros
@ref BIT <br>
@ref AR_BIND_EVENT_FN <br>
@ref AR_STATIC_BIND_EVENT_FN <br>
@ref AR_BIND_ASSET_FN <br>
@ref AR_STATIC_BIND_ASSET_FN <br>
@ref AR_BIND_SYSTEM_CALLBACK_FN

@subsection BIT
<h2 class="memtitle"><span class="permalink"><a href="#BIT">◆&nbsp;</a></span>BIT</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define BIT</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>x</em></span></td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		(1 << x)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro returns a value with the <code>x</code>th bit set to 1.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">x</td><td>The bit index to set.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_BIND_EVENT_FN
<h2 class="memtitle"><span class="permalink"><a href="#AR_BIND_EVENT_FN">◆&nbsp;</a></span>AR_BIND_EVENT_FN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_BIND_EVENT_FN</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>fn</em></span></td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		std::bind(&fn, this, std::placeholders::_1)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro creates a <code>std::bind</code> to bind the specified event handler function to the current object and passes the event as a parameter.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">fn</td><td>The event handler function.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_STATIC_BIND_EVENT_FN
<h2 class="memtitle"><span class="permalink"><a href="#AR_STATIC_BIND_EVENT_FN">◆&nbsp;</a></span>AR_STATIC_BIND_EVENT_FN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_STATIC_BIND_EVENT_FN</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>fn</em></span></td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		std::bind(&fn, std::placeholders::_1)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro creates a <code>std::bind</code> to bind a static event handler function to the current object and passes the event as a parameter.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">fn</td><td>The static event handler function.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_BIND_ASSET_FN
<h2 class="memtitle"><span class="permalink"><a href="#AR_BIND_ASSET_FN">◆&nbsp;</a></span>AR_BIND_ASSET_FN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_BIND_ASSET_FN</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>fn</em></span></td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		std::bind(&fn, this, std::placeholders::_1)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro creates a <code>std::bind</code> to bind the specified asset handler function to the current object and passes the asset as a parameter.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">fn</td><td>The asset handler function.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_STATIC_BIND_ASSET_FN
<h2 class="memtitle"><span class="permalink"><a href="#AR_STATIC_BIND_ASSET_FN">◆&nbsp;</a></span>AR_STATIC_BIND_ASSET_FN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_STATIC_BIND_ASSET_FN</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>fn</em></span></td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		std::bind(&fn, std::placeholders::_1)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro creates a <code>std::bind</code> to bind a static asset handler function to the current object and passes the asset as a parameter.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">fn</td><td>The static asset handler function.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>

@subsection AR_BIND_SYSTEM_CALLBACK_FN
<h2 class="memtitle"><span class="permalink"><a href="#AR_BIND_SYSTEM_CALLBACK_FN">◆&nbsp;</a></span>AR_BIND_SYSTEM_CALLBACK_FN</h2>
<div class="memitem">
	<div class="memproto">
    <table class="memname">
      <tbody>
				<tr>
          <td class="memname">#define AR_BIND_SYSTEM_CALLBACK_FN</td>
          <td>(</td>
          <td class="paramtype"></td>
					<td class="paramname">
						<span class="paramname"><em>fn, object</em></span>
					</td>
					<td>)</td>
        </tr>
      </tbody>
		</table>
	</div>
	<div class="memdoc">
		<b>Value:</b>
		@code{.cpp}
		std::bind(&fn, object, std::placeholders::_1)
		@endcode
		<div style="margin-block-start: 1em; margin-block-end: 1em; line-height: var(--content-line-height);">This macro creates a <code>std::bind</code> to bind a static asset handler function to the current object and passes the asset as a parameter.</div>
		<dl class="params">
			<dt>Parameters</dt>
			<dd>
				<table class="params">
					<tbody><tr><td class="paramname">fn</td><td>The static asset handler function.</td></tr>
					</tbody>
				</table>
  		</dd>
		</dl>
	</div>
</div>