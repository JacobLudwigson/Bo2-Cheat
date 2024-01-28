#pragma once
#include <vector>
#include <Windows.h>
#include <mutex>
#include <vector>
#include <TlHelp32.h>

namespace Memory
{
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	DWORD GetProcId(const wchar_t* procName);
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
}


class signature
{
public:

	signature(const std::string& sig);

	signature import(const std::string & module_name = ("PalWorld-Win64-Shipping.exe"));

	signature add(uint32_t offset);
	signature sub(uint32_t offset);
	signature instruction(uint32_t offset);

	signature dump(const std::string& func_name);

#ifdef _WIN64
	uint64_t GetPointer();
#else
	uint32_t GetPointer();
#endif

private:
#ifdef _WIN64
	uint64_t pointer;
#else
	uint32_t pointer;
#endif

	bool imported;
	std::string sig;
	std::string module_name;

};

