#include <string>


#include "lua_hooker.h"
#include "hooker.h"
#include "logger/clogger.h"


#define aLUA_GLOBALSINDEX        (-10002)
#define alua_getglobal(L,s)      AzurLaneL::lua_getfield(L, aLUA_GLOBALSINDEX, (s))
#define alua_setglobal(L,s)      AzurLaneL::lua_setfield(L, aLUA_GLOBALSINDEX, (s))
#define alua_pushcfunction(L,f)  AzurLaneL::lua_pushcclosure(L, (f), 0)
namespace LuaHooker {

    void registerCasLua(lua_State* L, lua_CFunction cPointer, char* luaFunction) {
        alua_pushcfunction(L, cPointer);
        Il2CppString* s = STR(luaFunction);
        alua_setglobal(L, (s));
    }


    //Deprecated
	void hookCtoLua(lua_State* L, lua_CFunction cPointer, char* objClass, char* luaFunction) {
        if (lua_gettop(L) != 0) {
            lua_settop(L, 0);
        }
        /**/
        logMessage("hookc", "PreObjectClass");
        if (objClass != NULL) {
            logMessage("hookc", "isnotnull");

            //lua_getglobal(L, STR(objClass));
            AzurLaneL::lua_getfield(L, LUA_GLOBALSINDEX, STR(objClass));
        }
        AzurLaneL::lua_getfield(L, LUA_GLOBALSINDEX, STR(luaFunction));
        logMessage("hookc", "pushclosure");
        AzurLaneL::lua_pushcclosure(L, cPointer, 0);
        logMessage("hookc", "setfield");
        AzurLaneL::lua_setfield(L, -2, STR(luaFunction));

        logMessage("hookc", "pop");
        if (objClass != NULL) {
            //lua_pop(L, 1);
            //lua_settop(L, -(n)-1
            AzurLaneL::lua_settop(L, 0);
        }

        logMessage("hookc", "end");
	}


}


namespace LuaExecutor {
    void (*loadString)(const char* str);
}