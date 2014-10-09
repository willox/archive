#include <string>

#include "GarrysMod/Lua/Interface.h"
#include "util/SigScan.h"

using namespace GarrysMod;

typedef void *(__cdecl *HandleChange_LuaFn)(std::string &dir, std::string &filename, std::string &ext);

HandleChange_LuaFn HandleChange_Lua;
const char *HandleChange_Lua_Signature = "\x55\x8B\xEC\x81\xEC????\x53\x56\x57\x8B\x7D\x10";


int Refresh(lua_State *state) {
	std::string filepath = "lua/";
	filepath.append(LUA->CheckString(1));

	// FIX SLASHES
	size_t pos = 0;

	while ((pos = filepath.find("\\", pos)) != std::string::npos) {
		filepath.replace(pos, 1, "/");

		pos++;
	}

	pos = 0;

	while ((pos = filepath.find("//", pos)) != std::string::npos) {
		filepath.replace(pos, 2, "/");

		pos++;
	}

	std::string dir = filepath.substr(0, filepath.find_last_of("/") + 1);
	std::string ext = filepath.substr(filepath.find_last_of(".") + 1);

	std::string filename = filepath.substr(filepath.find_last_of("/") + 1, filepath.length() - ext.length() - dir.length() - 1);

	HandleChange_Lua(dir, filename, ext);

	return 0;
}

GMOD_MODULE_OPEN() {
	SigScan server = SigScan("server");

	HandleChange_Lua = (HandleChange_LuaFn)server.Scan(HandleChange_Lua_Signature);

	if (!HandleChange_Lua) {
		LUA->ThrowError("HandleChange_Lua not found.");
		return 0;
	}

	LUA->PushSpecial(Lua::SPECIAL_GLOB);
		LUA->PushCFunction(Refresh);
		LUA->SetField(-2, "refresh");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}