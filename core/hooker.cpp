
#include "core/hooker.h"
#include "core/hook_util.h"
#include "core/hook_functions.h"
#include "logger/clogger.h"
#include <link.h>
#include <dlfcn.h>
#include <fcntl.h>
#include "Substrate/SymbolFinder.h"

#include "Context.h"
#include "core/my_cpparser.h"
#include "ModApi/my_lua_tolstring.h"
#include "lua_hooker.h"
#include "core/variables.h"



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>


namespace Function_Hooker {

    void load_lua_funcs();
    void load_il2cpp_funcs();
    Il2CppImage* get_image();

    // define il2cpp functions
    Il2CppDomain* (*il2cpp_domain_get)();
    Il2CppAssembly* (*il2cpp_domain_assembly_open)(void* domain, char* name);
    Il2CppImage* (*il2cpp_assembly_get_image)(void* assembly);
    void* (*il2cpp_class_from_name)(void* image, char* namespaze, char* name); 
    MethodInfo* (*il2cpp_class_get_method_from_name)(void* klass, char* name, int argsCount);
    Il2CppString* (*il2cpp_string_new)(char* str);
    Il2CppAssembly** (*il2cpp_domain_get_assemblies)(Il2CppDomain* domain, size_t* name);


    void load_lua_funcs() {
        //This can be obtained without relative address but it stoped working and i don't know why so now its fixed addr
        /*
        AzurLaneL::lua_getfield = (void (*)(lua_State*, int, Il2CppString*)) GETLUAFUNCBYADDR(LUA_GETFIELD_ADDR);
        AzurLaneL::lua_pushcclosure = (void (*)(lua_State*, lua_CFunction, int)) GETLUAFUNCBYADDR(LUA_PUSHCCLOSURE_ADDR);
        AzurLaneL::lua_setfield = (void (*)(lua_State*, int, Il2CppString*)) GETLUAFUNCBYADDR(LUA_SETFIELD_ADDR);
        AzurLaneL::lua_settop = (void (*)(lua_State*, int)) GETLUAFUNCBYADDR(LUA_SETTOP_ADDR);
        */
        AzurLaneL::lua_getfield = (void (*)(lua_State*, int, Il2CppString*)) GETLUAFUNC("lua_getfield");
        AzurLaneL::lua_pushcclosure = (void (*)(lua_State*, lua_CFunction, int)) GETLUAFUNC("lua_pushcclosure");
        AzurLaneL::lua_setfield = (void (*)(lua_State*, int, Il2CppString*)) GETLUAFUNC("lua_setfield");
        AzurLaneL::lua_settop = (void (*)(lua_State*, int)) GETLUAFUNC("lua_settop");

        AzurLaneL::lua_pcall = (int (*)(lua_State*, int, int, int)) /*(GETLUAFUNC("lua_pcall"));/**/GETLUAFUNCBYADDR(LUAL_PCALL_ADDR);/**/
        AzurLaneL::luaL_loadstring = (int (*)(lua_State * L, const char* s)) /*(GETLUAFUNC("luaL_loadstring"));/**/GETLUAFUNCBYADDR(LUAL_LOADSTRING_ADDR);/**/
        AzurLaneL::luaL_loadfile = (int (*)(lua_State*, const char*))/*(GETLUAFUNC("luaL_loadfile"));/**/(GETLUAFUNCBYADDR(LUAL_LOADFILE_ADDR));/**/

        //This must be fixed addr
        AzurLaneL::lj_lex_setup = (int (*)(lua_State*, LexState*)) GETLUAFUNCBYADDR(LJ_LEX_SETUP_ADDR);
        AzurLaneL::lj_bcread = (GCproto * (*)(LexState*)) GETLUAFUNCBYADDR(LJ_BCREAD_ADDR);
        AzurLaneL::lj_func_newL_empty = (GCfunc * (*)(lua_State*, GCproto*, GCtab*)) GETLUAFUNCBYADDR(LJ_FUNC_NEWL_EMPTY_ADDR);
        AzurLaneL::lj_err_str = (GCstr * (*)(lua_State*, ErrMsg)) GETLUAFUNCBYADDR(LJ_ERR_STR_ADDR);
        AzurLaneL::lj_err_throw = (void (*)(lua_State*, int)) GETLUAFUNCBYADDR(LJ_ERR_THROW_ADDR);
        /**/
    }
    void load_il2cpp_funcs() {
        // populate all il2cpp functions
        il2cpp_domain_get = (Il2CppDomain * (*)()) GETSYM(il2cppLibName, "il2cpp_domain_get");
        il2cpp_domain_assembly_open = (Il2CppAssembly * (*)(void*, char*)) GETSYM(il2cppLibName,
            "il2cpp_domain_assembly_open");
        il2cpp_assembly_get_image = (Il2CppImage * (*)(void*)) GETSYM(il2cppLibName,
            "il2cpp_assembly_get_image");
        logValue("testing","il2cpp_class_from_name prevalue: %p", il2cpp_class_from_name);
        il2cpp_class_from_name = (void* (*)(void*, char*, char*)) GETSYM(il2cppLibName,
            "il2cpp_class_from_name");
        logValue("testing", "il2cpp_class_from_name posvalue: %p", il2cpp_class_from_name);
        il2cpp_class_get_method_from_name = (MethodInfo * (*)(void*, char*, int)) GETSYM(
            il2cppLibName, "il2cpp_class_get_method_from_name");
        il2cpp_string_new = (Il2CppString * (*)(char*)) GETSYM(il2cppLibName,
            "il2cpp_string_new");
        il2cpp_domain_get_assemblies = (Il2CppAssembly * *(*)(Il2CppDomain*, size_t*)) GETSYM(il2cppLibName,
            "il2cpp_domain_get_assemblies");
    }

    Il2CppImage* get_image() {
        //il2cpp_domain_get_assemblies
        size_t size;
        Il2CppAssembly** assemblies;
        // call the functions necessary to get the image
        Il2CppDomain* domain = il2cpp_domain_get();
        Il2CppAssembly* assembly = il2cpp_domain_assembly_open(domain, "Assembly-CSharp");
        return (Il2CppImage*)(assembly->imageIndex);// il2cpp_assembly_get_image(assembly);
    }
    bool isLibraryLoaded(const char* libraryName) {
        //libLoaded = true;
        char line[512] = { 0 };
        FILE* fp = fopen(("/proc/self/maps"), ("rt"));
        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                std::string a = line;
                if (strstr(line, libraryName)) {
                    globvar::libLoaded = true;
                    return true;
                }
            }
            fclose(fp);
        }
        return false;
    }
    Il2CppMethodPointer* getFunctionAddress(char* namespaze, char* klass, char* method) {
        //logMessage("getFunctionAddress", "----------------------------------------:");
        //logMessage("getFunctionAddress", "getFunctionAddress:");
        logValue("getFunctionAddress", "Using image: %p",globvar::image);
        //logMessage("getFunctionAddress", namespaze);
        void* iklass = il2cpp_class_from_name(globvar::image, namespaze, klass);
        //logMessage("getFunctionAddress", klass);
        MethodInfo* imethod = il2cpp_class_get_method_from_name(iklass, method, -1);
        //logMessage("getFunctionAddress", method);
        logValue("getFunctionAddress", "imethod dir: %p", imethod);
        if (imethod) {
            logValue("getFunctionAddress", "imethod pointer: %p", imethod->methodPointer);
            return imethod->methodPointer;
        }
        logMessage("getFunctionAddress", "imethod pointer: NULL");
        return NULL;
    }/**/

    uintptr_t getLibraryBaseAddress(pid_t pid, const std::string& libraryName) {
        std::ostringstream mapsPath;
        mapsPath << "/proc/" << pid << "/maps";
        std::ifstream maps(mapsPath.str());
        std::string line;

        while (std::getline(maps, line)) {
            if (line.find(libraryName) != std::string::npos) {
                std::istringstream iss(line);
                uintptr_t start, end;
                iss >> std::hex >> start;
                iss.ignore(1); // Skip the hyphen
                iss >> std::hex >> end;
                return start;
            }
        }
        // 0 if not found
        return 0;
    }

    void print_function_memdir() 
    {
        logSeparator("funvalues");
        logMessage("funvalues","Il2CPP funvalues:");
        logValue("funvalues", "il2cpp_domain_get: %p", il2cpp_domain_get);
        logValue("funvalues", "il2cpp_domain_assembly_open: %p", il2cpp_domain_assembly_open);
        logValue("funvalues", "il2cpp_assembly_get_image: %p", il2cpp_assembly_get_image);
        logValue("funvalues", "il2cpp_class_from_name: %p", il2cpp_class_from_name);
        logValue("funvalues", "il2cpp_class_get_method_from_name: %p", il2cpp_class_get_method_from_name);
        logValue("funvalues", "il2cpp_string_new: %p", il2cpp_string_new);
        logValue("funvalues", "il2cpp_domain_get_assemblies: %p", il2cpp_domain_get_assemblies);
        logSeparator("funvalues");
        logMessage("funvalues", "Lua funvalues:");
        logValue("funvalues", "lua_pcall: %p", AzurLaneL::lua_pcall);
        logValue("funvalues", "luaL_loadstring: %p", AzurLaneL::luaL_loadstring);
        logValue("funvalues", "luaL_loadfile: %p", AzurLaneL::luaL_loadfile);
        logValue("funvalues", "lj_lex_setup: %p", AzurLaneL::lj_lex_setup);
        logValue("funvalues", "lj_bcread: %p", AzurLaneL::lj_bcread);
        logValue("funvalues", "lj_func_newL_empty: %p", AzurLaneL::lj_func_newL_empty);
        logValue("funvalues", "lj_err_str: %p", AzurLaneL::lj_err_str);
        logValue("funvalues", "lj_err_throw: %p", AzurLaneL::lj_err_throw);
        logValue("funvalues", "lj_parse: %p", lj_parse);
    }



    #define PORT 15156
    #define MAX_BUFFER_SIZE 1024
    // thread where everything is ran
    void* hook_thread(void*) {
        logSeparator("values");
        logValue("values", "compstr:", COMPSTR);
        logValue("values", "Formidable_ML Version: ",VERSION);
        logValue("values", "Build arch version: %s", BUILDVER);

        // check if target lib is loaded
        do {
            sleep(3);
        } while (!isLibraryLoaded(il2cppLibName) || !isLibraryLoaded(toLuaLibName));

        pid_t pid = getpid();
        logValue("values", "PID: %d", pid);
        globvar::luabase = getLibraryBaseAddress(pid, toLuaLibName);
        logValue("values", "LuaBaseLib: %p", globvar::luabase);
        uintptr_t il2cpp_base_addr = getLibraryBaseAddress(pid, il2cppLibName);
        logValue("values", "Il2CPPLib: %p", il2cpp_base_addr);

        // load necessary functions
        Function_Hooker::load_il2cpp_funcs();
        globvar::image =Function_Hooker::get_image();
        logValue("values", "Image memdir: %p", globvar::image);
        Function_Hooker::load_lua_funcs();
        Function_Hooker::print_function_memdir();

        /**/
        hook((void*)/**/GETLUAFUNC("lua_tolstring")/**/, (void*)my_lua_tolstring,
            (void**)&old_lua_tolstring);
        /**/

        hook((void*)(globvar::luabase + CPPARSER_ADDR), (void*)my_cpparser,
            (void**)&old_cpparser);/**/


       // AzurLaneL::lua_pcall = (int (*)(lua_State*, int, int, int)) /*(GETLUAFUNC("lua_pcall"));/**/;/**/


        //TODO move to another class
        if (!REMOTE_EXECUTION) {
            logMessage("modloader", "Remote code execution disabled, this feature is only enabled in dev version...");
            return nullptr;
        }
        else {
            logMessage("modloader", "Remote code execution enabled, use it only in a controled environment!!");
        }


        int server_socket, client_socket;
        struct sockaddr_in server_address, client_address;
        socklen_t address_len = sizeof(client_address);
        char buffer[MAX_BUFFER_SIZE];

        // Create socket
        if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            ///perror("Socket creation failed");
            return nullptr;
        }

        // Set up server address struct
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(PORT);

        // Bind the socket
        if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            logMessage("socket", "Socket binding failed");
            close(server_socket);
            return nullptr;
        }

        // Listen for incoming connections
        if (listen(server_socket, 5) == -1) {
            logMessage("socket", "Socket listening failed");
            close(server_socket);
            return nullptr;
        }

        logValue("socket", "Server listening on port %d...", PORT);

        // Accept connections and handle data
        while (1) {
            logMessage("socket", "Server listening clients");
            // Accept a connection
            if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &address_len)) == -1) {
                logMessage("socket", "Socket accepting failed");
                close(server_socket);
                return nullptr;
                //exit(EXIT_FAILURE);
            }
            logMessage("socket", "Client Conected");

            // Receive data from the client
            ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                logMessage("socket", "Error receiving data");
                close(client_socket);
                continue;
            }

            // Null-terminate the received data
            buffer[bytes_received] = '\0';

            if (globvar::mod_loaded) {
                logMessage("socket", "Remote code execution");
                LuaExecutor::loadString(buffer);
            }
            else {
                logMessage("socket", "Discarted code, modloader is not loaded!");
            }
            
            // Close the client socket
            close(client_socket);
        }

        // Close the server socket
        close(server_socket);












        return nullptr;
    }
}