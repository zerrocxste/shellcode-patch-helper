#include <iostream>
#include <Windows.h>

#ifdef _WIN64
#define DWORD_OF_BITNESS DWORD64
#define PTRMAXVAL ((PVOID)0x000F000000000000)
#elif _WIN32
#define DWORD_OF_BITNESS DWORD
#define PTRMAXVAL ((PVOID)0xFFF00000)
#endif

#include "utilites/utilites.h"
#include "shellcode_patch_helper/shellcode_patch_helper.h"