
#ifndef LUA_HOOKER
#define LUA_HOOKER

#include "tolua_runtime/src/lua.hpp"
#include "tolua_runtime/src/lj_frame.h"
#include "tolua_runtime/src/lj_lex.h"
#include "tolua_runtime/src/lj_parse.h"
#include "hook_functions.h"

namespace LuaHooker {

	void hookCtoLua(lua_State* L, lua_CFunction cPointer, char* objClass, char* luaFunction);
	void registerCasLua(lua_State* L, lua_CFunction cPointer, char* luaFunction);
}

namespace LuaExecutor {
	extern void (*loadString)(const char* str);
}
#endif // !LUA_HOOKER
