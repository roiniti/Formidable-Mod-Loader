#ifndef HOOK_UTIL
#define HOOK_UTIL

#include <vector>
#include <string>
#include <Structs.h>
#include "hooker.h"

//Target lib
#define il2cppLibName "libil2cpp.so"
#define toLuaLibName "libtolua.so"


#define GETLUAFUNC(method) Function_Hooker::getFunctionAddress(("LuaInterface"), ("LuaDLL"), (method))
//#define GETLUAFUNCBYADDR(addr) (void*) (luabase+addr)
#define GETLUAFUNCBYADDR(addr) (Il2CppMethodPointer*) (globvar::luabase+addr)



std::vector<std::string> split(const std::string& s, char delim);
bool isLibraryLoaded(const char* libraryName);

void loadil2cppfuncs();
void crash();



#if defined(__aarch64__) //Compile for arm64 lib only
#include <And64InlineHook/And64InlineHook.hpp>

#else //Compile for armv7 lib only.
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

#endif

void hook(void* offset, void* ptr, void** orig)
{
#if defined(__aarch64__)
    A64HookFunction(offset, ptr, orig);
#else
    MSHookFunction(offset, ptr, orig);
#endif
}

#define HOOK(offset, ptr, orig) hook((void *)getAbsoluteAddress(il2cppLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB(lib, offset, ptr, orig) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

#define HOOK_NO_ORIG(offset, ptr) hook((void *)getAbsoluteAddress(il2cppLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

#define GETSYM(lib, sym) dlsym(dlopen(lib, 4), (sym))

#define HOOKSYM(sym, ptr, org) hook(GETSYM(il2cppLibName, sym), (void *)ptr, (void **)&org)
#define HOOKSYM_LIB(lib, sym, ptr, org) hook(GETSYM(OBFUSCATE(lib), sym), (void *)ptr, (void **)&org)

#define HOOKSYM_NO_ORIG(sym, ptr)  hook(GETSYM(il2cppLibName, sym), (void *)ptr, NULL)
#define HOOKSYM_LIB_NO_ORIG(lib, sym, ptr) hook(GETSYM(OBFUSCATE(lib), sym), (void *)ptr, NULL)



#endif // !HOOK_UTIL
