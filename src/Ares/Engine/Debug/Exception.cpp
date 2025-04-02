#include <arespch.h>
#include "Engine/Debug/Exception.h"

#include "Engine/Utility/String.h"

namespace Ares {

	namespace Internal {

		namespace ErrorMessages {

			const char* g_NullPointer =				"[Moderate]   Pointer Error  1004 - Attempted to use a null pointer.";
			const char* g_DerefNullPointer =		"[  High  ]   Pointer Error  1016 - Attempted to dereference a null pointer.";
			const char* g_InvalidPointerAccess =	"[Moderate]   Pointer Error  1024 - Accessed an invalid or uninitialized pointer.";
			const char* g_PointerOutOfBounds =		"[ Severe ]   Pointer Error  1037 - Accessed memory outside of allocated bounds.";
 
			const char* g_Memory =					"[  Minor ]   Memory Error   2003 - General memory-related issue.";
			const char* g_BadAlloc =				"[Critical]   Memory Error   2018 - Memory allocation failed.";
			const char* g_OutOfMemory =				"[  Fatal ]   Memory Error   2029 - System out of memory.";
			const char* g_InvalidMemoryAccess =		"[Critical]   Memory Error   2038 - Accessed restricted or invalid memory.";
 
			const char* g_File =					"[  Minor ]    File Error    3003 - General file-related issue.";
			const char* g_FileNotFound =			"[  Major ]    File Error    3015 - File not found.";
			const char* g_FileRead =				"[  High  ]    File Error    3026 - Could not read file.";
			const char* g_FileWrite =				"[  High  ]    File Error    3036 - Could not write to file.";
 
			const char* g_Network =					"[  Minor ]   Network Error  4003 - General network-related issue.";
			const char* g_ConnectionFailed =		"[ Severe ]   Network Error  4017 - Failed to establish a network connection.";
			const char* g_DataTransFailed =			"[ Severe ]   Network Error  4027 - Failed to transmit data.";
			const char* g_Timeout =					"[  High  ]   Network Error  4036 - Network operation timed out.";

			const char* g_Validation =				"[Moderate] Validation Error 5004 - General validation-related issue.";
			const char* g_InvalidInput =			"[  Major ] Validation Error 5015 - Invalid input received.";
			const char* g_DataCorruption =			"[  Fatal ] Validation Error 5029 - Data corruption detected.";

			const char* g_Logic =					"[Moderate]    Logic Error   6004 - General logic-related issue.";
			const char* g_InvalidState =			"[  High  ]    Logic Error   6016 - Program reached an invalid state.";
			const char* g_UnexpectedCondition =		"[ Severe ]    Logic Error   6027 - Unexpected condition encountered.";

			const char* g_Permission =				"[  Minor ] Permission Error 7003 - General permission-related issue.";
			const char* g_AccessDenied =			"[  High  ] Permission Error 7016 - Access to resource denied.";
			const char* g_InsuffientPermissions =	"[  Major ] Permission Error 7025 - Insufficient permissions for the operation.";

			const char* g_UnknownError =			"[Critical]   Unknown Error  - An unknown error has occurred!";

			char g_CustomError[256];

		}

		constexpr const char* GetErrorDescription(const ErrorCode code)
		{
			switch (code)
			{
			case ErrorCode::NULL_POINTER: return "Attempted to use a null pointer.";
			case ErrorCode::DEREF_NULL_POINTER: return "Attempted to dereference a null pointer.";
			case ErrorCode::INVALID_POINTER_ACCESS: return "Accessed an invalid or uninitialized pointer.";
			case ErrorCode::POINTER_OUT_OF_BOUNDS: return "Accessed memory outside of allocated bounds.";

			case ErrorCode::MEMORY: return "General memory-related issue.";
			case ErrorCode::BAD_ALLOC: return "Memory allocation failed.";
			case ErrorCode::OUT_OF_MEMORY: return "System out of memory.";
			case ErrorCode::INVALID_MEMORY_ACCESS: return "Accessed restricted or invalid memory.";

			case ErrorCode::FILE: return "General file-related issue.";
			case ErrorCode::FILE_NOT_FOUND: return "File not found.";
			case ErrorCode::FILE_READ: return "Could not read file.";
			case ErrorCode::FILE_WRITE: return "Could not write to file.";

			case ErrorCode::NETWORK: return "General network-related issue.";
			case ErrorCode::CONNECTION_FAILED: return "Failed to establish a network connection.";
			case ErrorCode::DATA_TRANS_FAILED: return "Failed to transmit data.";
			case ErrorCode::TIMEOUT: return "Network operation timed out.";

			case ErrorCode::VALIDATION: return "General validation-related error.";
			case ErrorCode::INVALID_INPUT: return "Invalid input received.";
			case ErrorCode::DATA_CORRUPTION: return "Data corruption detected.";

			case ErrorCode::LOGIC: return "General logic-related issue.";
			case ErrorCode::INVALID_STATE: return "Program reached an invalid state.";
			case ErrorCode::UNEXPECTED_CONDITION: return "Unexpected condition encountered.";

			case ErrorCode::PERMISSION: return "General permission-related issue.";
			case ErrorCode::ACCESS_DENIED: return "Access to resource denied.";
			case ErrorCode::INSUFFICIENT_PERMISSIONS: return "Insufficient permissions for the operation.";

			default: return "An unknown error has occurred!";
			}
		}

		constexpr const char* GetErrorSeverity(const ErrorCode severity)
		{
			const uint16_t severityNum = static_cast<uint16_t>(severity) % 10;
			switch (severityNum)
			{
			case 1: return "[ Trivial]";
			case 2: return "[   Low  ]";
			case 3: return "[  Minor ]";
			case 4: return "[Moderate]";
			case 5: return "[  Major ]";
			case 6: return "[  High  ]";
			case 7: return "[ Severe ]";
			case 8: return "[Critical]";
			case 9: return "[  Fatal ]";
			default: return "[ Unknown]";
			}
		}

		constexpr const char* GetErrorTitle(const ErrorCode code)
		{
			switch (static_cast<uint16_t>(code) / 1000)
			{
			case 1: return "   Pointer Error  ";
			case 2: return "   Memory Error   ";
			case 3: return "    File Error    ";
			case 4: return "   Network Error  ";
			case 5: return " Validation Error ";
			case 6: return "    Logic Error   ";
			case 7: return " Permission Error ";
			default: return "   Unknown Error  ";
			}
		}

		

		constexpr const char* GetErrorMessage(const ErrorCode code)
		{
			switch (code)
			{
			case ErrorCode::NULL_POINTER: return Internal::ErrorMessages::g_NullPointer;
			case ErrorCode::DEREF_NULL_POINTER: return Internal::ErrorMessages::g_DerefNullPointer;
			case ErrorCode::INVALID_POINTER_ACCESS: return Internal::ErrorMessages::g_InvalidPointerAccess;
			case ErrorCode::POINTER_OUT_OF_BOUNDS: return Internal::ErrorMessages::g_PointerOutOfBounds;

			case ErrorCode::MEMORY: return Internal::ErrorMessages::g_Memory;
			case ErrorCode::BAD_ALLOC: return Internal::ErrorMessages::g_BadAlloc;
			case ErrorCode::OUT_OF_MEMORY: return Internal::ErrorMessages::g_OutOfMemory;
			case ErrorCode::INVALID_MEMORY_ACCESS: return Internal::ErrorMessages::g_InvalidMemoryAccess;

			case ErrorCode::FILE: return Internal::ErrorMessages::g_File;
			case ErrorCode::FILE_NOT_FOUND: return Internal::ErrorMessages::g_FileNotFound;
			case ErrorCode::FILE_READ: return Internal::ErrorMessages::g_FileRead;
			case ErrorCode::FILE_WRITE: return Internal::ErrorMessages::g_FileWrite;

			case ErrorCode::NETWORK: return Internal::ErrorMessages::g_Network;
			case ErrorCode::CONNECTION_FAILED: return Internal::ErrorMessages::g_ConnectionFailed;
			case ErrorCode::DATA_TRANS_FAILED: return Internal::ErrorMessages::g_DataTransFailed;
			case ErrorCode::TIMEOUT: return Internal::ErrorMessages::g_Timeout;

			case ErrorCode::VALIDATION: return Internal::ErrorMessages::g_Validation;
			case ErrorCode::INVALID_INPUT: return Internal::ErrorMessages::g_InvalidInput;
			case ErrorCode::DATA_CORRUPTION: return Internal::ErrorMessages::g_DataCorruption;

			case ErrorCode::LOGIC: return Internal::ErrorMessages::g_Logic;
			case ErrorCode::INVALID_STATE: return Internal::ErrorMessages::g_InvalidState;
			case ErrorCode::UNEXPECTED_CONDITION: return Internal::ErrorMessages::g_UnexpectedCondition;

			case ErrorCode::PERMISSION: return Internal::ErrorMessages::g_Permission;
			case ErrorCode::ACCESS_DENIED: return Internal::ErrorMessages::g_AccessDenied;
			case ErrorCode::INSUFFICIENT_PERMISSIONS: return Internal::ErrorMessages::g_InsuffientPermissions;

			default: return Internal::ErrorMessages::g_UnknownError;
			}
		}

		Exception::Exception(ErrorCode errorCode)
			: m_ErrorCode(errorCode)
		{
			std::memset(m_ErrorMessage, 0, sizeof(m_ErrorMessage));
			Utility::StringConcat(m_ErrorMessage, GetErrorSeverity(errorCode));
			Utility::StringConcat(m_ErrorMessage, GetErrorTitle(errorCode));
			Utility::StringAppendInteger(m_ErrorMessage, static_cast<uint32_t>(errorCode));
			Utility::StringConcat(m_ErrorMessage, " - ");
			Utility::StringConcat(m_ErrorMessage, GetErrorDescription(errorCode));
		}

		const char* Exception::what() const noexcept
		{
			return m_ErrorMessage;
		}

	}

}

namespace eastl {

	size_t hash<Ares::ErrorCode>::operator()(const Ares::ErrorCode& errorCode) const
	{
		return static_cast<size_t>(errorCode);
	}

}