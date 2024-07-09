#ifndef HOOKFUNCTIONS
#define HOOKFUNCTIONS


#include "tolua_runtime/src/lua.hpp"
#include "tolua_runtime/src/lj_frame.h"
#include "tolua_runtime/src/lj_lex.h"
#include "tolua_runtime/src/lj_parse.h"
#include "Structs.h"

namespace AzurLaneL {


	//API needed
	extern void (*lua_getfield)(lua_State*, int, Il2CppString*);
	extern void (*lua_pushcclosure)(lua_State*, lua_CFunction, int);
	extern void (*lua_setfield)(lua_State*, int, Il2CppString*);
	extern void (*lua_settop)(lua_State*, int);


	//Modloader needed
	extern int (*lua_pcall)(lua_State*, int, int, int);
	extern int (*luaL_loadstring)(lua_State* L, const char* s);
	extern int (*luaL_loadfile)(lua_State* L, const char* s);



	//Core needed
	extern int (*lj_lex_setup)(lua_State*, LexState*);
	extern GCproto* (*lj_bcread)(LexState*);
	extern GCfunc* (*lj_func_newL_empty)(lua_State*, GCproto*, GCtab*);
	extern GCstr* (*lj_err_str)(lua_State*, ErrMsg);
	extern void (*lj_err_throw)(lua_State*, int);

}


#endif