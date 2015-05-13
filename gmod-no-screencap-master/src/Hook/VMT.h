#ifndef VHOOKS_VMT_H
#define VHOOKS_VMT_H

namespace Hook {

	class VMT {
	public:

		VMT(void *target);

		template< typename HookFn >
		HookFn GetOriginal(int index) {
			return (HookFn)(VTable_Old[index]);
		}

		void Hook(int index, void *hook);
		void Unhook(int index);

		void Remove();

	private:

		~VMT();

		size_t VTable_Size = 0;

		void **VTable_Old;
		void **VTable;
	};
}


#endif