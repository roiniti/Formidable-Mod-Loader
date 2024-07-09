#include "hook_functions.h"


namespace AzurLaneL {

	//API needed
	void (*lua_getfield)(lua_State*, int, Il2CppString*);
	void (*lua_pushcclosure)(lua_State*, lua_CFunction, int);
	void (*lua_setfield)(lua_State*, int, Il2CppString*);
	void (*lua_settop)(lua_State*, int);


	//Modloader needed
	int (*lua_pcall)(lua_State*, int, int, int);
	int (*luaL_loadstring)(lua_State* L, const char* s);
	int (*luaL_loadfile)(lua_State* L, const char* s);

	
	//Core needed
	int (*lj_lex_setup)(lua_State*, LexState*);
	GCproto* (*lj_bcread)(LexState*);
	GCfunc* (*lj_func_newL_empty)(lua_State*, GCproto*, GCtab*);
	GCstr* (*lj_err_str)(lua_State*, ErrMsg);
	void (*lj_err_throw)(lua_State*, int);

}