#ifndef _UTIL_SIGSCAN_H
#define _UTIL_SIGSCAN_H

class SigScan {
private:
	char *base_addr;
	size_t base_len;

public:
	typedef char Object;

	SigScan(void *base_addr, size_t base_len);
	SigScan(const char *module_name);
	SigScan(void *insider_addr);
	

	Object *Scan(const char *signature, int offset = 0);
	static Object *CalcAbsAddress(Object *address);
	bool IsValid();

	char *GetBase();
	size_t GetLength();
};

#endif