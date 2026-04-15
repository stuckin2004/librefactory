#include "lua_manager.hpp"
#include <windows.h>
#include <stdio.h>

lua_State *gL = NULL;

static int l_Log(lua_State *L)
{
	const char* msg = luaL_checkstring(L, 1);
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
	return 0;
}

static int l_Quit(lua_State *L)
{
	PostQuitMessage(0);
	return 0;
}

static int l_IsKeyDown(lua_State *L)
{
	/* stubbed -> need wndproc key state table later */
	lua_pushboolean(L, 0);
	return 1;
}

/*
 * This is the part where we scan for mods, the base Lua file and get all
 * of that setup nicely for the player. Take note that the following file
 * 
 * "mods\base\main.lua"
 *
 * And its supporting scripts are NOT exactly optional, as that's... the core
 * of the game, much like Factorio. If you remove or edit the Base mod, you're
 * completely on your own with just the exposed functions.
 *
 * Also, modifying the above functions prefixed with "l_" also puts you on your
 * own. Since that's modifying engine code directly and can lead to undefined
 * results and behaviour.
 *
 */

static void RegisterInternalAPIs(lua_State *L)
{
	/* stub */
}

static void LoadModsFromDirectory(lua_State *L)
{
	/* Always check for Base first. Completely bail if missing. */
	HANDLE hBaseFile = CreateFileA(
		".\\mods\\base\\main.lua",
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		
	if (hBaseFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL,
			"mods\\base\\main.lua is missing!"
			"\n"
			"This file has core game functionality built-in.\n"
			"Please double-check your install or replace\n"
			"the script ASAP. The game cannot continue.",
			"Missing core files",
			MB_OK | MB_ICONERROR);
		
		CloseHandle(hBaseFile);
		ExitProcess((UINT)1);
	}
}