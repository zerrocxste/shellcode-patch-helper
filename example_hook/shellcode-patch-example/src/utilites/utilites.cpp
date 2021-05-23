#include "../includes.h"

DWORD_OF_BITNESS compare_mem(const char* pattern, const char* mask, DWORD_OF_BITNESS base, DWORD_OF_BITNESS size, const int patternLength, DWORD speed)
{
	for (DWORD_OF_BITNESS i = 0; i < size - patternLength; i += speed)
	{
		bool found = true;
		for (DWORD_OF_BITNESS j = 0; j < patternLength; j++)
		{
			if (mask[j] == '?')
				continue;

			if (pattern[j] != *(char*)(base + i + j))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}

DWORD_OF_BITNESS get_module_size(DWORD_OF_BITNESS address)
{
	return PIMAGE_NT_HEADERS(address + (DWORD_OF_BITNESS)PIMAGE_DOS_HEADER(address)->e_lfanew)->OptionalHeader.SizeOfImage;
}

DWORD_OF_BITNESS find_pattern(HMODULE module, const char* pattern, const char* mask, DWORD scan_speed)
{
	DWORD_OF_BITNESS base = (DWORD_OF_BITNESS)module;
	DWORD_OF_BITNESS size = get_module_size(base);

	DWORD_OF_BITNESS patternLength = (DWORD_OF_BITNESS)strlen(mask);

	return compare_mem(pattern, mask, base, size, patternLength, scan_speed);
}