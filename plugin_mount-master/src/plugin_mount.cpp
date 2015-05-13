#include "plugin_mount.h"
#include <string>

#include "tier2.h"
#include "engine/iserverplugin.h"
#include "FileSystem.h"

#define CONTENT_BASE "E:/garrysmod/content"

CPluginMount g_pPluginMount;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CPluginMount, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_pPluginMount);

bool CPluginMount::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn serverFactory) {
	ConnectTier2Libraries(&interfaceFactory, 1);

	auto fs = g_pFullFileSystem;

	fs->AddSearchPath(CONTENT_BASE, "game", PATH_ADD_TO_HEAD);
	fs->AddSearchPath(CONTENT_BASE, "mod", PATH_ADD_TO_HEAD);
	fs->AddSearchPath(CONTENT_BASE "/lua", "lsv", PATH_ADD_TO_HEAD);
	
	FileFindHandle_t hFind;

	auto strMatch = fs->FindFirst(CONTENT_BASE "/addons/*", &hFind);

		do {
			if (strcmp(".", strMatch) == 0 || strcmp("..", strMatch) == 0)
				continue;

			std::string match = CONTENT_BASE "/addons/";
			match.append(strMatch);

			if (fs->FindIsDirectory(hFind)) {
				fs->AddSearchPath(match.c_str(), "game");
				match.append("/lua");
				fs->AddSearchPath(match.c_str(), "lsv");
			}
		} while (strMatch = fs->FindNext(hFind));

	fs->FindClose(hFind);

	return true;
}

void CPluginMount::Unload() {

}

void CPluginMount::Pause() {

}

void CPluginMount::UnPause() {

}

const char *CPluginMount::GetPluginDescription() {
	return "PLUGIN_MOUNT";
}

void CPluginMount::LevelInit(char const *pMapName) {

}

void CPluginMount::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {

}

void CPluginMount::GameFrame(bool simulating) {

}

void CPluginMount::LevelShutdown() {

}

void CPluginMount::ClientActive(edict_t *pEntity) {

}

void CPluginMount::ClientDisconnect(edict_t *pEntity) {

}

void CPluginMount::ClientPutInServer(edict_t *pEntity, char const *pPlayerName) {

}

void CPluginMount::SetCommandClient(int index) {

}

void CPluginMount::ClientSettingsChanged(edict_t *pEdict) {

}

PLUGIN_RESULT CPluginMount::ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) {
	return PLUGIN_CONTINUE;
}

PLUGIN_RESULT CPluginMount::ClientCommand(edict_t *pEntity, const CCommand &args) {
	return PLUGIN_CONTINUE;
}

PLUGIN_RESULT CPluginMount::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) {
	return PLUGIN_CONTINUE;
}

void CPluginMount::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {

}

void CPluginMount::OnEdictAllocated(edict_t *pEdict) {

}

void CPluginMount::OnEdictFreed(const edict_t *edict) {

}