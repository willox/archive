#include "SigScan.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <Windows.h>
	#include <Psapi.h>
#else
	// TODO
#endif

SigScan::SigScan(void *base_addr, size_t base_len)
{
	this->base_addr = (char *) base_addr;
	this->base_len = base_len;
}

SigScan::SigScan(void *insider_addr)
{
	if (!insider_addr) {
		return;
	}

#ifdef _WIN32
	MEMORY_BASIC_INFORMATION mem;

	if (!VirtualQuery(insider_addr, &mem, sizeof(mem))) {
		return;
	}

	base_addr = (char *) mem.AllocationBase;

	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *) mem.AllocationBase;
	IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS *) ((unsigned long) dos + (unsigned long) dos->e_lfanew);

	if(pe->Signature != IMAGE_NT_SIGNATURE) {
		base_addr = nullptr;
		return;
	}

	base_len = (size_t) pe->OptionalHeader.SizeOfImage;
#else
	// TODO
#endif
}

SigScan::SigScan(const char *module_name)
{
#ifdef _WIN32
	HMODULE module = GetModuleHandle(module_name);

	if (module) {
		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

		base_addr = (char *) info.lpBaseOfDll;
		base_len = (size_t) info.SizeOfImage;
	}
#else
	// TODO
#endif
}

SigScan::Object *SigScan::Scan(const char *signature, int offset)
{
	if (!IsValid()) {
		return nullptr;
	}

	int loc = 0;
	
	for (SigScan::Object *ptr = base_addr; ptr < base_addr + base_len; ptr++) {
		if (signature[loc] == '?' || signature[loc] == *ptr) {
			if (signature[loc + 1] == '\0') {
				return ptr - loc + offset;
			}

			loc++;
		} else {
			loc = 0;
		}
	}
	
	return nullptr;
}

SigScan::Object *SigScan::CalcAbsAddress(SigScan::Object *address)
{
	return address + *(signed long *)address + 4;
}

bool SigScan::IsValid()
{
	if (base_addr && base_len) {
		return true;
	}

	return false;
}

char *SigScan::GetBase()
{
	return base_addr;
}

size_t SigScan::GetLength()
{
	return base_len;
}