#ifndef MY_CPPARSER
#define MY_CPPARSER

#include "tolua_runtime/src/lua.hpp"
#include "tolua_runtime/src/lj_frame.h"
#include "tolua_runtime/src/lj_lex.h"
#include "tolua_runtime/src/lj_parse.h"

extern TValue* (*old_cpparser)(lua_State* instance, lua_CFunction dummy, void* ud);
TValue* my_cpparser(lua_State* L, lua_CFunction dummy, void* ud);



#endif // !MY_CPPARSER
