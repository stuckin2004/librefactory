#ifndef LUA_MANAGER_HPP
#define LUA_MANAGER_HPP

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

/* Since Lua is builtin, the VM init should never fail
   but just in case it does we return a boolean */
bool LuaInit();

/* Call into a future defined tick() function with dT in seconds. */
void LuaTick(float dt);

/* Tear down and close the Lua VM */
void LuaShutdown();

/* Global Lua VM handle; exposed so API mods can use if/when needed */
extern lua_State *gL;

#endif /* LUA_MANAGER_HPP */