#include <assert.h>
#include <Windows.h>

#include "cdll_int.h"
#include "materialsystem/imaterial.h"

#include "Hook/VTable.h"
#include "Hook/VMT.h"
#include "SigScan.h"

typedef void *(*FactoryFn)(const char *name, int ret);
typedef void (__thiscall *CMatRenderContext_ReadPixelsFn)(void *_this, int x, int y, int w, int h, unsigned char *buffer, void *format);

const int CMaterialSystem_GetRenderContext_Offset = 98;
const int CMatRenderContext_ReadPixels_Offset = 13;

Hook::VMT *CMatRenderContext_Hooks;

IVEngineClient *client;

void __fastcall CMatRenderContext_ReadPixels_Hook(IMatRenderContext *_this, void *, int x, int y, int w, int h, unsigned char *buffer, void *format) {
	_this->ClearColor3ub(255, 0, 255);
	_this->ClearBuffers(true, true);

	// TODO: Draw penis

	client->ClientCmd_Unrestricted("say Hey, don't take screenshots of me!\n");

	return CMatRenderContext_Hooks->GetOriginal<CMatRenderContext_ReadPixelsFn>(CMatRenderContext_ReadPixels_Offset)(_this, x, y, w, h, buffer, format);
}

DWORD WINAPI Load(LPVOID lpParameter) {
	HMODULE engine;
	HMODULE materialsystem;	
	
	while (!(engine = GetModuleHandle("engine.dll")));
	while (!(materialsystem = GetModuleHandle("materialsystem.dll")));
	
	auto EngineFactory = (FactoryFn)GetProcAddress(engine, "CreateInterface");
	client = (IVEngineClient *)EngineFactory("VEngineClient013", 0);

	auto MaterialSystemFactory = (FactoryFn)GetProcAddress(materialsystem, "CreateInterface");
	auto *material = (IMaterialSystem *)MaterialSystemFactory(MATERIAL_SYSTEM_INTERFACE_VERSION, 0);

	auto RenderContext = material->GetRenderContext();

	CMatRenderContext_Hooks = new Hook::VMT(RenderContext);
	CMatRenderContext_Hooks->Hook(CMatRenderContext_ReadPixels_Offset, CMatRenderContext_ReadPixels_Hook);

	return 0;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, Load, NULL, 0, NULL);
	}

	return true;
}
