#ifndef CLOGGER
#define CLOGGER
#include <string>

/*
_CRT_STDIO_INLINE int __CRTDECL sprintf(
    _Pre_notnull_ _Always_(_Post_z_) char* const _Buffer,
    _In_z_ _Printf_format_string_    char const* const _Format,
    ...)*/
void logMessage(const char* tag, const char* message);
void logSeparator(const char* tag);
void logValue(const char* tag, const char* strformat, ...);



#endif // !CLOGGER
