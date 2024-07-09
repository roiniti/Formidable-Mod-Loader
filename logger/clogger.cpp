
#include "clogger.h"
#include <android/log.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>


void logValue(const char* tag, const char* strformat, ...) {
    // Calculate the required buffer size
    va_list args;
    va_start(args, strformat);
    size_t len = vsnprintf(nullptr, 0, strformat, args);
    va_end(args);

    // Allocate memory for the formatted string
    char* msg = (char*)malloc(len + 1);
    if (msg == nullptr) {
        // Handle allocation failure
        return;
    }

    // Format the string
    va_start(args, strformat);
    vsnprintf(msg, len + 1, strformat, args);
    va_end(args);

    // Log the message
    logMessage(tag, msg);

    // Free the allocated memory
    free(msg);
}

void logMessage(const char* tag, const char* message) {
    //return;
    // Define el nivel de registro (por ejemplo, ANDROID_LOG_DEBUG, ANDROID_LOG_INFO, ANDROID_LOG_ERROR)
    int logLevel = ANDROID_LOG_ERROR;

    // Registra el mensaje utilizando el Android NDK Logger
    __android_log_write(logLevel, tag, message);
}


void logSeparator(const char* tag) {
    logMessage(tag, "----------------------------------------------");
}