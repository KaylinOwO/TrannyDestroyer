#include <Windows.h>
#include "Interfaces.h"
#include "Hooks.h"


DWORD WINAPI dwCheatInit(LPVOID lpArgs)
{
	while (!GetModuleHandle("client.dll"))
		Sleep(1000);

	g_Interfaces.Init();
	g_Hooks.Init();

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_Hooks.CheatModule = hModule;
		CreateThread(NULL, NULL, dwCheatInit, NULL, NULL, NULL);
	}
	return true;
}