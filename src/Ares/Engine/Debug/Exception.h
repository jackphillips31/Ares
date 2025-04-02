#pragma once

namespace Ares {

	enum class ErrorCode : uint16_t
	{
		NULL_POINTER = 1004,
		DEREF_NULL_POINTER = 1016,
		INVALID_POINTER_ACCESS = 1024,
		POINTER_OUT_OF_BOUNDS = 1037,
		MEMORY = 2003,
		BAD_ALLOC = 2018,
		OUT_OF_MEMORY = 2029,
		INVALID_MEMORY_ACCESS = 2038,
		FILE = 3003,
		FILE_NOT_FOUND = 3015,
		FILE_READ = 3026,
		FILE_WRITE = 3036,
		NETWORK = 4003,
		CONNECTION_FAILED = 4017,
		DATA_TRANS_FAILED = 4027,
		TIMEOUT = 4036,
		VALIDATION = 5004,
		INVALID_INPUT = 5015,
		DATA_CORRUPTION = 5029,
		LOGIC = 6004,
		INVALID_STATE = 6016,
		UNEXPECTED_CONDITION = 6027,
		PERMISSION = 7003,
		ACCESS_DENIED = 7016,
		INSUFFICIENT_PERMISSIONS = 7025
	};

	namespace Internal {

		class Exception : public std::exception
		{
		public:
			Exception(ErrorCode errorCode);
			virtual const char* what() const noexcept override;

		private:
			ErrorCode m_ErrorCode;
			char m_ErrorMessage[256] = {};
		};

	}


	namespace Error {

		class InvalidArgument : public Internal::Exception
		{
		public:
			InvalidArgument() : Exception(ErrorCode::INVALID_INPUT) {}
		};

	}

}

namespace eastl {

	template <>
	struct hash<Ares::ErrorCode>
	{
		size_t operator()(const Ares::ErrorCode& errorCode) const;
	};

}