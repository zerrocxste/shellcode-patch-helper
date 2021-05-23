#include "includes.h"

DWORD64 structure_pointer;
class c_vars_class
{
public:
	float shit1arr[10];
	float shit2;
	int shit3;
	int shit4;
	int m_count;
	int shitarr5[100];
}*g_vars_class;

void thread(void* arg1)
{
	std::unique_ptr<CShellCodeHelper>m_pShellcodeHelper = std::make_unique<CShellCodeHelper>();

	DWORD64 addr = find_pattern(GetModuleHandle(NULL), "\x89\x53", "xx", 0x1); // Address of signature = program-example.exe + 0x000014CE, "\x89\x53", "xx", "89 53"

	if (addr == NULL)
	{
		std::cout << "something wrong\n";
		return;
	}

	std::cout << "instruction found: 0x" << std::hex << addr << std::endl;

	auto setup_status = m_pShellcodeHelper->setup((void*)addr);

	if (!setup_status)
		return;

	BYTE shellcode[] =
	{
		0xc7, 0x43, 0x34, 0x05, 0x00, 0x00, 0x00,						// mov[rbx + 34], 00000005
		0x83, 0xFA, 0x0A,												// cmp edx, 0A					(sohranenno s originala potomychto razmer instrucii < 5)
		0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,		// mov rax, address
		0x48, 0x89, 0x18												// mov [rax], rbx
	};

	auto address_value = (DWORD64)&structure_pointer;
	memcpy(shellcode + 12, &address_value, 8);

	auto patch_status = m_pShellcodeHelper->patch(shellcode, sizeof(shellcode), 6);

	if (!patch_status)
		return;

	Sleep(1000);

	while (true)
	{
		g_vars_class = (c_vars_class*)structure_pointer;
		if (g_vars_class != NULL && !IsBadReadPtr(g_vars_class, sizeof(g_vars_class)))
		{
			std::cout << std::hex << "structure pointer: 0x" << g_vars_class << std::dec <<
				", g_vars_class->m_count: " << g_vars_class->m_count <<
				", offset from start structure to value: 0x" << std::hex << offsetof(c_vars_class, m_count) << std::dec << std::endl;
		}
		else
		{
			std::cout << std::hex << "structure pointer: 0x" << g_vars_class << std::dec << std::endl;
		}
		Sleep(1000);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, hModule, NULL, NULL);
	}

	return TRUE;
}

