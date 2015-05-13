#include <Windows.h>

#include "VMT.h"
#include "VTable.h"

namespace Hook {

	VMT::VMT(void *target) {
		VTable = GetVTable(target);
		VTable_Size = GetVTableSize(VTable);

		VTable_Old = new void *[VTable_Size];

		for (size_t i = 0; i < VTable_Size; i++) {
			VTable_Old[i] = VTable[i];
		}
	}

	VMT::~VMT() {
		DWORD old_protection;

		VirtualProtect(VTable, VTable_Size, PAGE_READWRITE, &old_protection);

		for (size_t i = 0; i < VTable_Size; i++) {
			VTable[i] = VTable_Old[i];
		}

		VirtualProtect(VTable, VTable_Size, old_protection, &old_protection);

		delete[] VTable_Old;
	}

	void VMT::Hook(int index, void *hook) {
		DWORD old_protection;

		VirtualProtect(VTable, VTable_Size, PAGE_READWRITE, &old_protection);
		VTable[index] = hook;
		VirtualProtect(VTable, VTable_Size, old_protection, NULL);
	}

	void VMT::Unhook(int index) {
		DWORD old_protection;

		VirtualProtect(VTable, VTable_Size, PAGE_READWRITE, &old_protection);
		VTable[index] = VTable_Old[index];
		VirtualProtect(VTable, VTable_Size, old_protection, NULL);
	}

	void VMT::Remove() {
		delete this;
	}

}