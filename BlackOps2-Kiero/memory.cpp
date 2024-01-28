#include "memory.h"

uintptr_t Memory::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		if (*(uintptr_t*)addr == NULL)
		{
			return NULL;
		}
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}


	return addr;
}
void Memory::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}


void Memory::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

std::uint8_t* find_sig(LPCSTR module_name, const std::string& byte_array)
{
	HMODULE module = GetModuleHandleA(module_name);

	if (!module)
		return nullptr;

	static const auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + std::strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(std::strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	const auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(byte_array.c_str());
	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	const auto pattern_size = pattern_bytes.size();
	const auto pattern_data = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - pattern_size; ++i)
	{
		auto found = true;

		for (auto j = 0ul; j < pattern_size; ++j)
		{
			if (scan_bytes[i + j] == pattern_data[j] || pattern_data[j] == -1)
				continue;
			found = false;
			break;
		}
		if (!found)
			continue;
		return &scan_bytes[i];
	}

	return nullptr;
}

signature::signature(const std::string& sig)
{
	this->imported = false;
	this->sig = sig;
}

signature signature::import(const std::string & module_name)
{
	this->imported = true;
	this->module_name = module_name;
	this->pointer = (uint64_t)find_sig(this->module_name.data(), this->sig);
	return *this;
}

signature signature::add(uint32_t value)
{
	if (!this->imported)
		*this = this->import();

	this->pointer += value;
	return *this;
}

signature signature::sub(uint32_t value)
{
	if (!this->imported)
		*this = this->import();

	this->pointer -= value;
	return *this;
}

signature signature::dump(const std::string& func_name)
{
	if (!this->imported)
		*this = this->import();

	return *this;
}

signature signature::instruction(uint32_t offset)
{
	if (!this->imported)
		*this = this->import();

	this->pointer = *(int*)(this->pointer + offset) + this->pointer;
	return *this;
}

#ifdef _WIN64
uint64_t signature::GetPointer()
{
	if (!this->imported)
		*this = this->import();
	return this->pointer;
}
#else
uint32_t signature::GetPointer()
{
	if (!this->imported)
		*this = this->import();
	return this->pointer;
}
#endif

DWORD Memory::GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}