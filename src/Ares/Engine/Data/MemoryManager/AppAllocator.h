#pragma once
#include <EASTL/allocator.h>

namespace Ares::Internal {

	class MemoryManager;

	class AppAllocator : public eastl::allocator
	{
	public:
		AppAllocator();
		EASTL_ALLOCATOR_EXPLICIT AppAllocator(const char* pName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
		EASTL_ALLOCATOR_EXPLICIT AppAllocator(MemoryManager* manager);
		EASTL_ALLOCATOR_EXPLICIT AppAllocator(MemoryManager& manager);

		AppAllocator(const AppAllocator& other);
		AppAllocator(const AppAllocator& other, const char* pName);
		AppAllocator& operator=(const AppAllocator& other);

		void* allocate(size_t size, int flags = 0) const;
		void* allocate(size_t size, size_t alignment, size_t offset, int flags = 0) const;
		void deallocate(void* ptr, size_t size = 0) const;

		const char* get_name() const;
		void set_name(const char* pName);

		//bool operator==(const AppAllocator& other) { return m_Manager == other.m_Manager; }
		//bool operator!=(const AppAllocator& other) { return m_Manager != other.m_Manager; }

		MemoryManager* GetManager() const { return m_Manager; }

	private:
		friend bool operator==(const AppAllocator&, const AppAllocator&);
		friend bool operator!=(const AppAllocator&, const AppAllocator&);

	private:
		MemoryManager* m_Manager;
	#if EASTL_NAME_ENABLED
		const char* m_pName;
	#endif
	};

}

namespace eastl {

	inline Ares::Internal::AppAllocator* get_default_allocator(const Ares::Internal::AppAllocator*)
	{
		return Ares::Internal::GetDefaultAllocator();
	}

	inline bool operator==(const Ares::Internal::AppAllocator& a, const Ares::Internal::AppAllocator& b)
	{
		return a.GetManager() == b.GetManager();
	}

	inline bool operator!=(const Ares::Internal::AppAllocator& a, const Ares::Internal::AppAllocator& b)
	{
		return a.GetManager() != b.GetManager();
	}

}