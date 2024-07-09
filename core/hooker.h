#ifndef CORE_HOOKER
#define CORE_HOOKER
#include "Structs.h"
namespace Function_Hooker {
	void* hook_thread(void*);
	Il2CppMethodPointer* getFunctionAddress(char* namespaze, char* klass, char* method);
	extern Il2CppString* (*il2cpp_string_new)(char* str);
}
#define STR(str) Function_Hooker::il2cpp_string_new((str))
#endif // !CORE_HOOKER
