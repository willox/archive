#include "Memory.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

namespace Memory {
	void Write(void *dest, const void *src, size_t src_size) {
		unsigned long old_protection;

		VirtualProtect(dest, src_size, PAGE_READWRITE, &old_protection);
		memcpy(dest, src, src_size);
		VirtualProtect(dest, src_size, old_protection, &old_protection);
	}
}