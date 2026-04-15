#include "lua_manager.hpp"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
    #include <unistd.h>
    #include <string.h>
#endif

lua_State *gL = NULL;

static int l_Log(lua_State *L)
{
    const char* msg = luaL_checkstring(L, 1);
#ifdef _WIN32
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
#else
    fprintf(stderr, "%s\n", msg);
#endif
    return 0;
}

static int l_Quit(lua_State *L)
{
#ifdef _WIN32
    PostQuitMessage(0);
#else
    _exit(0);
#endif
    return 0;
}

static int l_IsKeyDown(lua_State *L)
{
    /* stubbed -> need wndproc/platform key state table later */
    lua_pushboolean(L, 0);
    return 1;
}

/*
 * This is the part where we scan for mods, the base Lua file and get all
 * of that setup nicely for the player. Take note that the following file
 *
 * "mods/base/main.lua"
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

static void FatalMissingBase()
{
#ifdef _WIN32
    MessageBoxA(NULL,
        "mods\\base\\main.lua is missing!\n"
        "\n"
        "This file has core game functionality built-in.\n"
        "Please double-check your install or replace\n"
        "the script ASAP. The game cannot continue.",
        "Missing core files",
        MB_OK | MB_ICONERROR);
    ExitProcess((UINT)1);
#else
    fprintf(stderr,
        "FATAL: mods/base/main.lua is missing!\n"
        "This file has core game functionality built-in.\n"
        "Please double-check your install or replace "
        "the script ASAP. The game cannot continue.\n");
    _exit(1);
#endif
}

static void RegisterInternalAPIs(lua_State *L)
{
    /* stub */
}

static void LoadModsFromDirectory(lua_State *L)
{
    /* Always check for Base first. Completely bail if missing. */
#ifdef _WIN32
    HANDLE hBaseFile = CreateFileA(
        ".\\mods\\base\\main.lua",
        GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hBaseFile == INVALID_HANDLE_VALUE)
    {
        FatalMissingBase();
        return;
    }
    CloseHandle(hBaseFile);
#else
    if (access("mods/base/main.lua", F_OK) != 0)
    {
        FatalMissingBase();
        return;
    }
#endif

    /* Load base first */
    if (luaL_dofile(L, "mods/base/main.lua") != LUA_OK)
    {
        fprintf(stderr, "base mod error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    /* Scan mods/ for everything else */
#ifdef _WIN32
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA("mods\\*", &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
        if (strcmp(fd.cFileName, ".") == 0)  continue;
        if (strcmp(fd.cFileName, "..") == 0) continue;
        if (strcmp(fd.cFileName, "base") == 0) continue;

        char path[MAX_PATH];
        wsprintfA(path, "mods\\%s\\main.lua", fd.cFileName);

        if (luaL_dofile(L, path) != LUA_OK)
        {
            OutputDebugStringA("mod error: ");
            OutputDebugStringA(lua_tostring(L, -1));
            OutputDebugStringA("\n");
            lua_pop(L, 1);
        }
    }
    while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
#else
    DIR *dir = opendir("mods");
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type != DT_DIR)        continue;
        if (strcmp(entry->d_name, ".") == 0)    continue;
        if (strcmp(entry->d_name, "..") == 0)   continue;
        if (strcmp(entry->d_name, "base") == 0) continue;

        char path[512];
        snprintf(path, sizeof(path), "mods/%s/main.lua", entry->d_name);

        if (luaL_dofile(L, path) != LUA_OK)
        {
            fprintf(stderr, "mod error (%s): %s\n",
                entry->d_name, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    closedir(dir);
#endif
}