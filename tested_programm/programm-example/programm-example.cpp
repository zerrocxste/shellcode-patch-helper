#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <map>

bool is_key_up(int key)
{
	static std::map<int, short>key_stat;

	auto& key_last_status = key_stat[key];

	auto ret = false;

	auto status = GetAsyncKeyState(key);

	if (status && !key_last_status)
		ret = true;

	key_last_status = GetAsyncKeyState(key);

	return ret;
}

static bool stop_work = false;

class c_vars_class
{
public:
	float shit1arr[10];
	float shit2;
	int shit3;
	int shit4;
	int m_count;
	int shitarr5[100];
};

void key_handler_thread()
{
	while (true)
	{
		if (is_key_up(VK_SPACE))
			stop_work = !stop_work, std::cout << "[+]stoped cycle routine: " << std::boolalpha << stop_work << std::endl;
		Sleep(1);
	}
}

void function()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)key_handler_thread, NULL, NULL, NULL);

	c_vars_class* g_vars_class = new c_vars_class;

	char title[50] = { 0 };

	sprintf(title, "g_vars_class pointer: %I64X", g_vars_class);

	SetConsoleTitle(title);

	while (true)
	{
		if (stop_work)
			continue;

		g_vars_class->m_count += 1;

		if (g_vars_class->m_count > 10)
			g_vars_class->m_count = 0;

		std::cout << g_vars_class->m_count << std::endl;

		Sleep(500);
	}

	delete g_vars_class;
}

int main()
{
	function();
}