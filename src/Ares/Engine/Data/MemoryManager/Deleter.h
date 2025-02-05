#pragma once

namespace Ares::Internal {

	class AppAllocator;

	struct Deleter
	{
	public:
		Deleter();
		Deleter(const AppAllocator* alloc);
		Deleter(const Deleter& other);
		Deleter& operator=(const Deleter& other);

		void operator()(void* ptr);

	private:
		const AppAllocator* m_Allocator;
	};

}