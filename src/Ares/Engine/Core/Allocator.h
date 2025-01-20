#pragma once

void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, const char* pName, int flags, unsigned debugFlags, const char* file, int line) noexcept;
void operator delete[](void* ptr, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);