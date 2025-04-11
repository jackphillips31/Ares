#pragma once

namespace Ares::Internal {

	class MemoryManager;

	class AppAllocator
	{
	public:
		AppAllocator();
		EASTL_ALLOCATOR_EXPLICIT AppAllocator(const char* name = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
		EASTL_ALLOCATOR_EXPLICIT AppAllocator(MemoryManager* manager);

		AppAllocator(const AppAllocator& other);
		AppAllocator(const AppAllocator& other, const char* name);
		AppAllocator& operator=(const AppAllocator& other);

		void* allocate(size_t size, int flags = 0) const;
		void* allocate(size_t size, size_t alignment, size_t offset, int flags = 0) const;
		void deallocate(void* ptr, size_t size = 0) const;

		const char* get_name() const;
		void set_name(const char* name);

		bool operator==(const AppAllocator& other) { return m_Manager == other.m_Manager; }
		bool operator!=(const AppAllocator& other) { return m_Manager != other.m_Manager; }

		MemoryManager* GetManager() const { return m_Manager; }

	private:
		MemoryManager* m_Manager;
	#if AR_BUILD_DEBUG
		const char* m_Name;
	#endif
	};

}

namespace eastl {

	Ares::Internal::AppAllocator* get_default_allocator(const Ares::Internal::AppAllocator*);

}