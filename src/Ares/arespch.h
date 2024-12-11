#pragma once

// Standard Library
#include <iostream>				// I/O streams
#include <fstream>				// File operations
#include <string>				// String handling
#include <sstream>				// String streams
#include <array>				// Fixed-size arrays
#include <vector>				// Dynamic arrays
#include <unordered_map>		// Hash map
#include <unordered_set>		// Hash set
#include <functional>			// Function objects
#include <algorithm>			// Algorithms
#include <bitset>				// Bit manipulation
#include <typeindex>			// Type information

#include <future>				// Asynchronous programming
#include <queue>				// Queue data structure
#include <thread>				// Threading
#include <mutex>				// Mutex for synchronization
#include <condition_variable>	// Condition variables
#include <atomic>				// Atomic operations

// Internal headers (e.g., engine-specific)
#include "Engine/Debug/Log.h"	// Logging functionality

// Platform-specific includes (Windows)
#ifdef AR_PLATFORM_WINDOWS
#include <Windows.h>			// Windows-specific functionality
#endif