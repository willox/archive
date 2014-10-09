#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include "util/SigScan.h"
#include "util/Memory.h"

DWORD WINAPI Load(LPVOID lpParameter) {
	// Wait for menusystem to have been loaded
	while (!GetModuleHandle("menusystem.dll"));

	// Crude memory replacements, keep override equal length or shorter
	// This is a better solution than giving out near-lua-bypasses
	const char *default_url = "asset://garrysmod/html/loading.html";
	const char *default_url_replacement = "asset://garrysmod/bin/loading.html";

	const char *game_details = "GameDetails";
	const char *game_details_replacement = "tostring";

	SigScan menusystem = SigScan("menusystem.dll");

	SigScan::Object *default_url_mem = menusystem.Scan(default_url);
	SigScan::Object *game_details_mem = menusystem.Scan(game_details);

	if (!default_url_mem || !game_details_mem) {
		return 0; // Silently die, if users realise it isn't working they can complain/update
	}

	Memory::Write(default_url_mem, default_url_replacement, strlen(default_url_replacement) + 1);
	Memory::Write(game_details_mem, game_details_replacement, strlen(game_details_replacement) + 1);

	return 0;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);

		// Don't want source throwing out our module now, do we?
		char module_name[MAX_PATH];
		GetModuleFileName(hModule, module_name, sizeof(module_name));
		GetModuleHandleEx(NULL, module_name, &hModule);

		CreateThread(0, 0, Load, NULL, 0, NULL);
	}

	return true;
}
