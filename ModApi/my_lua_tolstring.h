#ifndef MY_LUA_TOLSTRING
#define MY_LUA_TOLSTRING

#include "tolua_runtime/src/lua.hpp"
#include "tolua_runtime/src/lj_frame.h"
#include "tolua_runtime/src/lj_lex.h"
#include "tolua_runtime/src/lj_parse.h"


extern void (*old_lua_tolstring)(lua_State* instance, int index, int& strLen);
void my_lua_tolstring(lua_State* instance, int index, int& strLen);
#endif // !MY_LUA_TOLSTRING
