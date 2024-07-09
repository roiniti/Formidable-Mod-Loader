
#include "my_lua_tolstring.h"

#ifndef IMPORT_TEST
#define IMPORT_TEST
#include "core/hook_functions.h"
#endif // !IMPORT_TEST
#include "core/lua_hooker.h"
#include "logger/clogger.h"
#include "core/variables.h"
#include "ModApi/ModReader.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "Apicode.h"

#define DISABLED false

void (*old_lua_tolstring)(lua_State* instance, int index, int& strLen);
#define azur_luaL_dofile(L, fn) \
	(AzurLaneL::luaL_loadfile(L, fn) || AzurLaneL::lua_pcall(L, 0, LUA_MULTRET, 0))

#define azur_luaL_dostring(L, fn) \
	(AzurLaneL::luaL_loadstring(L, fn) || AzurLaneL::lua_pcall(L, 0, LUA_MULTRET, 0))

void include(lua_State* L, const char* name);
void print_mgr(lua_State* L, std::string message);
void include(lua_State* L, const char* path) 
{
    std::string mods = "/mods/";
    mods.append(path);
    std::string fullpath = "";
    fullpath.append(globvar::home_route);
    fullpath.append(mods);
    azur_luaL_dofile(L, fullpath.c_str());
}
int hook_include(lua_State* L);
int load_api(lua_State* L);


int hook_include(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    include(L, path);
    return 0;
}
int android_log(lua_State* L) {
    const char* message = lua_tostring(L, 1);
    logValue("lualog", message);
    return 0;
}
int android_log_t(lua_State* L) {
    const char* tag = lua_tostring(L, 1);
    const char* message = lua_tostring(L, 2);
    logValue(tag, message);
    return 0;
}
lua_State* gL;
void lString(const char* str) {
    azur_luaL_dostring(gL, str);

};


int load_api(lua_State* L) {
    gL = L;
    LuaExecutor::loadString = lString;
    //logMessage("apiloader", "Loading cinclude...");
    LuaHooker::registerCasLua(L, hook_include, "cinclude");
    //logMessage("apiloader", "Loading alog...");
    LuaHooker::registerCasLua(L, android_log, "alog");
    //logMessage("apiloader", "Loading alogt...");
    LuaHooker::registerCasLua(L, android_log_t, "alogt");
    //logMessage("apiloader", "Loading dummy...");
    azur_luaL_dostring(L, DUMMY_FUNCTION);
    //logMessage("apiloader", "Loading printMgr...");
    azur_luaL_dostring(L, PRINT_FUNCTION);
    //logMessage("apiloader", "Loading include...");
    azur_luaL_dostring(L, INCLUDE_FUNCTION);
    //logMessage("apiloader", "Loading tab to string...");
    azur_luaL_dostring(L, TAB_TO_STRING_FUNCTION);
    //logMessage("apiloader", "Loading table to string...");
    azur_luaL_dostring(L, TABLE_TO_STRING_FUNCTION);

    return 0;
}

void print_mgr(lua_State* L,std::string message) {
    std::string luaCode = "pg.TipsMgr.GetInstance():ShowTips('"+message+"')";
    int result = AzurLaneL::luaL_loadstring(L, luaCode.c_str());
    AzurLaneL::lua_pcall(L, 0, 0, 0);
}


int simple_load(lua_State* L,std::string path) {
    /*
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string archivo = entry.path().filename().string();
            std::string extension = entry.path().extension().string();

            if (extension == ".lua" && archivo != "main.lua") {
                azur_luaL_dofile(L,entry.path().string().c_str());
            }
        }
    }
    */
    return azur_luaL_dofile(L, (path+"/main.lua").c_str());
    
}

//This method need to be updated
// If the modloader tries to load code before some azur lane code its loaded it will make the game to crash
// a better code load detection will be implemented for now when the game calls lua_tolstring the code its loaded
// the game needs to call it no times before on versions 5.1-7.x and 1 times for 8.x-current
//0 for 5.1-7.x, 1 for 8.x+
int tolstring_until_load = 1;
void my_lua_tolstring(lua_State* L, int index, int& strLen) {



    if (DISABLED) {
        logMessage("modloader", "Modloader is disabled");
        return old_lua_tolstring(L, index, strLen);
    }
    if (!globvar::mod_loaded) {
        if (tolstring_until_load != 0) {
            logValue("modloader", "need %d calls more to be loaded", tolstring_until_load);
            tolstring_until_load--;
            return old_lua_tolstring(L, index, strLen);
        }
        


        globvar::mod_loaded = true;
        logMessage("modloader", "Loading lua modapi...");
        load_api(L);
        logMessage("modloader", "Checking for mod files...");
        std::unique_ptr<std::list<AzurModInfo>> mod_info=read_mods_info(globvar::home_route+"/mods");
        
        
        if (mod_info == nullptr)
        {
            logMessage("modloader", "No mods found...");
            print_mgr(L, "No mods found...");
        }else for (const AzurModInfo& info : *(mod_info)) {
            logValue("modloader", "Loading %s",info.modname.c_str());
            int status = simple_load(L, info.mod_path);
            logValue("modloader", "%s load complete! Status:%d", info.modname.c_str(), status);
            logMessage("modloader", "loaded!");
            print_mgr(L, info.mod_path + " Loaded!");
        }



        logMessage("modloader", "Check complete!");
    }
    return old_lua_tolstring(L, index, strLen);
}