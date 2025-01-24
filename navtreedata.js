/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Ares", "index.html", [
    [ "Overview", "index.html", "index" ],
    [ "Getting Started", "getting_started.html", [
      [ "Prerequisites", "getting_started.html#autotoc_md0", null ],
      [ "Cloning the Repository", "getting_started.html#autotoc_md1", null ],
      [ "Generating Project Files", "getting_started.html#autotoc_md2", null ]
    ] ],
    [ "Usage", "usage.html", [
      [ "Create a New Application", "usage.html#autotoc_md3", [
        [ "Basic Application Setup", "usage.html#autotoc_md4", null ],
        [ "Customizing Application Settings", "usage.html#autotoc_md6", null ]
      ] ],
      [ "Adding Layers", "usage.html#autotoc_md8", null ]
    ] ],
    [ "Event System", "event_system.html", [
      [ "Overview", "event_system.html#autotoc_md9", null ],
      [ "Key Features", "event_system.html#autotoc_md10", null ],
      [ "How to Use the Event System", "event_system.html#autotoc_md11", [
        [ "Initialization and Shutdown", "event_system.html#autotoc_md12", null ],
        [ "Processing Events", "event_system.html#autotoc_md13", null ],
        [ "Dispatching Events", "event_system.html#autotoc_md14", null ],
        [ "Adding Listeners", "event_system.html#autotoc_md15", null ],
        [ "Removing Listeners", "event_system.html#autotoc_md16", null ]
      ] ],
      [ "Relevant Documentation", "event_system.html#autotoc_md17", null ]
    ] ],
    [ "Asset Manager", "asset_manager.html", [
      [ "Overview", "asset_manager.html#autotoc_md18", null ],
      [ "Key Features", "asset_manager.html#autotoc_md19", null ],
      [ "How to Use the Asset Manager", "asset_manager.html#autotoc_md20", [
        [ "Initialization and Shutdown", "asset_manager.html#autotoc_md21", null ],
        [ "Staging Assets", "asset_manager.html#autotoc_md22", null ],
        [ "Loading and Unloading Assets", "asset_manager.html#autotoc_md23", [
          [ "Loading Assets", "asset_manager.html#autotoc_md24", null ],
          [ "Unloading Assets", "asset_manager.html#autotoc_md25", null ]
        ] ],
        [ "Accessing Assets", "asset_manager.html#autotoc_md26", null ],
        [ "Asset Listener System", "asset_manager.html#autotoc_md27", [
          [ "Adding Listeners", "asset_manager.html#autotoc_md28", null ],
          [ "Removing Listeners", "asset_manager.html#autotoc_md29", null ]
        ] ]
      ] ],
      [ "Relevant Documentation", "asset_manager.html#autotoc_md30", null ]
    ] ],
    [ "Macros", "macros.html", [
      [ "Introduction", "macros.html#macros_intro", [
        [ "Platform Macros", "macros.html#macros_platform_defines", null ],
        [ "Processor Architecture Macros", "macros.html#macros_processor_defines", null ],
        [ "Endianness Macros", "macros.html#macros_endian_defines", null ],
        [ "Compiler and Assembly Macros", "macros.html#macros_compiler_defines", null ],
        [ "System Configuration Macros", "macros.html#macros_system_defines", null ],
        [ "Debug and Assert Macros", "macros.html#macros_debug_defines", null ],
        [ "Utility Macros", "macros.html#macros_utility_defines", null ]
      ] ],
      [ "Platform Macros", "platform_defines.html", [
        [ "List of Macros", "platform_defines.html#list_platform_defines", [
          [ "AR_PLATFORM_MOBILE", "platform_defines.html#AR_PLATFORM_MOBILE", null ],
          [ "AR_PLATFORM_DESKTOP", "platform_defines.html#AR_PLATFORM_DESKTOP", null ],
          [ "AR_PLATFORM_CONSOLE", "platform_defines.html#AR_PLATFORM_CONSOLE", null ],
          [ "AR_PLATFORM_WINDOWS", "platform_defines.html#AR_PLATFORM_WINDOWS", null ],
          [ "AR_PLATFORM_MINGW", "platform_defines.html#AR_PLATFORM_MINGW", null ],
          [ "AR_PLATFORM_WINRT", "platform_defines.html#AR_PLATFORM_WINRT", null ],
          [ "AR_PLATFORM_LINUX", "platform_defines.html#AR_PLATFORM_LINUX", null ],
          [ "AR_PLATFORM_CYGWIN", "platform_defines.html#AR_PLATFORM_CYGWIN", null ],
          [ "AR_PLATFORM_OSX", "platform_defines.html#AR_PLATFORM_OSX", null ],
          [ "AR_PLATFORM_IPHONE", "platform_defines.html#AR_PLATFORM_IPHONE", null ],
          [ "AR_PLATFORM_ANDROID", "platform_defines.html#AR_PLATFORM_ANDROID", null ],
          [ "AR_PLATFORM_PS4", "platform_defines.html#AR_PLATFORM_PS4", null ],
          [ "AR_PLATFORM_XBOXONE", "platform_defines.html#AR_PLATFORM_XBOXONE", null ]
        ] ]
      ] ],
      [ "Processor Architecture Macros", "processor_defines.html", [
        [ "List of Macros", "processor_defines.html#list_processor_defines", [
          [ "AR_PROCESSOR_X86", "processor_defines.html#AR_PROCESSOR_X86", null ],
          [ "AR_PROCESSOR_X86_64", "processor_defines.html#AR_PROCESSOR_X86_64", null ],
          [ "AR_PROCESSOR_ARM32", "processor_defines.html#AR_PROCESSOR_ARM32", null ],
          [ "AR_PROCESSOR_ARM64", "processor_defines.html#AR_PROCESSOR_ARM64", null ],
          [ "AR_PROCESSOR_POWERPC", "processor_defines.html#AR_PROCESSOR_POWERPC", null ],
          [ "AR_PROCESSOR_POWERPC_32", "processor_defines.html#AR_PROCESSOR_POWERPC_32", null ],
          [ "AR_PROCESSOR_POWERPC_64", "processor_defines.html#AR_PROCESSOR_POWERPC_64", null ]
        ] ]
      ] ],
      [ "Endianness Macros", "endianness_defines.html", [
        [ "List of Macros", "endianness_defines.html#list_endianness_defines", [
          [ "AR_SYSTEM_LITTLE_ENDIAN", "endianness_defines.html#AR_SYSTEM_LITTLE_ENDIAN", null ],
          [ "AR_SYSTEM_BIG_ENDIAN", "endianness_defines.html#AR_SYSTEM_BIG_ENDIAN", null ]
        ] ]
      ] ],
      [ "Compiler and Assembly Macros", "compiler_defines.html", [
        [ "List of Macros", "compiler_defines.html#list_compiler_defines", [
          [ "AR_ASM_STYLE_ATT", "compiler_defines.html#AR_ASM_STYLE_ATT", null ],
          [ "AR_ASM_STYLE_INTEL", "compiler_defines.html#AR_ASM_STYLE_INTEL", null ]
        ] ]
      ] ],
      [ "System Configuration Macros", "system_defines.html", [
        [ "List of Macros", "system_defines.html#list_system_defines", [
          [ "AR_PLATFORM_PTR_SIZE", "system_defines.html#AR_PLATFORM_PTR_SIZE", null ],
          [ "AR_PLATFORM_WORD_SIZE", "system_defines.html#AR_PLATFORM_WORD_SIZE", null ],
          [ "AR_PLATFORM_MIN_MALLOC_ALIGNMENT", "system_defines.html#AR_PLATFORM_MIN_MALLOC_ALIGNMENT", null ],
          [ "AR_MISALIGNED_SUPPORT_LEVEL", "system_defines.html#AR_MISALIGNED_SUPPORT_LEVEL", null ],
          [ "AR_CACHE_LINE_SIZE", "system_defines.html#AR_CACHE_LINE_SIZE", null ]
        ] ]
      ] ],
      [ "Debug and Assert Macros", "debug_defines.html", [
        [ "List of Macros", "debug_defines.html#list_debug_defines", [
          [ "AR_BUILD_DEBUG", "debug_defines.html#AR_BUILD_DEBUG", null ],
          [ "AR_ENABLE_ASSERTS", "debug_defines.html#AR_ENABLE_ASSERTS", null ],
          [ "AR_ENABLE_PROFILING", "debug_defines.html#AR_ENABLE_PROFILING", null ],
          [ "AR_ASSERT", "debug_defines.html#AR_ASSERT", null ],
          [ "AR_CORE_ASSERT", "debug_defines.html#AR_CORE_ASSERT", null ]
        ] ]
      ] ],
      [ "Utility Macros", "utility_defines.html", [
        [ "List of Macros", "utility_defines.html#list_utility_defines", [
          [ "BIT", "utility_defines.html#BIT", null ],
          [ "AR_BIND_EVENT_FN", "utility_defines.html#AR_BIND_EVENT_FN", null ],
          [ "AR_STATIC_BIND_EVENT_FN", "utility_defines.html#AR_STATIC_BIND_EVENT_FN", null ],
          [ "AR_BIND_ASSET_FN", "utility_defines.html#AR_BIND_ASSET_FN", null ],
          [ "AR_STATIC_BIND_ASSET_FN", "utility_defines.html#AR_STATIC_BIND_ASSET_FN", null ]
        ] ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ],
        [ "Enumerator", "namespacemembers_eval.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"Application_8h.html",
"classAres_1_1Application.html#a860a36da09de99a7e812a9bf0c5d57da",
"index.html",
"structAres_1_1WindowProps.html#a2a53cb4f05dee41754e2efc866585771"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';