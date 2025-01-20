#pragma once

// EA Standard Library
#include <EABase/eabase.h>
#include <EASTL/allocator.h>
#include <EASTL/string.h>

// Standard Library
#include <iostream>				// I/O streams
#include <fstream>				// File operations
//#include <string>				// String handling
#include <sstream>				// String streams
#include <array>				// Fixed-size arrays
#include <vector>				// Dynamic arrays
#include <unordered_map>		// Hash map
#include <unordered_set>		// Hash set
#include <functional>			// Function objects
#include <algorithm>			// Algorithms
#include <bitset>				// Bit manipulation
#include <typeindex>			// Type information
#include <regex>				// Regular expressions
#include <variant>				// Utility library
#include <type_traits>			// Metaprogramming
#include <cstdio>				// C standard library
#include <stdio.h>				// Generic file operation

#include <future>				// Asynchronous programming
#include <queue>				// Queue data structure
#include <thread>				// Threading
#include <mutex>				// Mutex for synchronization
#include <shared_mutex>			// Shared mutex for synchronization
#include <condition_variable>	// Condition variables
#include <atomic>				// Atomic operations

// Internal headers (e.g., engine-specific)
#include "Engine/Debug/Log.h"	// Logging functionality

// Platform-specific includes (Windows)
#ifdef AR_PLATFORM_WINDOWS
#include <Windows.h>			// Windows-specific functionality
#endif