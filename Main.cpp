#include <list>
#include <vector>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <thread>
#include <jni.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <link.h>
#include <android/log.h>

#include "tolua_runtime/src/lua.hpp"
#include "tolua_runtime/src/lj_frame.h"
#include "tolua_runtime/src/lj_lex.h"
#include "tolua_runtime/src/lj_parse.h"


#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/Toast.hpp"
#include "Structs.h"


#include "core/hooker.h"
#include "logger/clogger.h"
#include "core/hook_functions.h"
#include "core/variables.h"
#include <errno.h>



//Sorry if this code is awfull bad coded etc
//Its my first large project in C++ and first public code code
//Code is based on Perseus (https://github.com/Egoistically/Perseus)
// 
//For building you will need android ndk 
//I use "ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application.mk" with all sources on a folder named jni
// 
//Also you will need to compile a custom version of libluajit (version 2.1) with the opcodes to match azur lane's opcodes
//I wont distribute that library just in case (legal reasons), the code inside here is mine excep from some parts
//Like perseus code or https://github.com/Rprop/And64InlineHook https://github.com/joeyjurjens/Android-Hooking-Template
//So no azur lane code is distributed here



std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}


void crash() {
    int* p = 0;
    *p = 0;
}


std::string getConfigPath(JNIEnv* env, jobject context) {
    jclass cls_Env = env->FindClass(("android/app/NativeActivity"));
    jmethodID mid = env->GetMethodID(cls_Env, ("getExternalFilesDir"),
        ("(Ljava/lang/String;)Ljava/io/File;"));
    jobject obj_File = env->CallObjectMethod(context, mid, NULL);
    jclass cls_File = env->FindClass(("java/io/File"));
    jmethodID mid_getPath = env->GetMethodID(cls_File, ("getPath"),
        ("()Ljava/lang/String;"));
    auto obj_Path = (jstring)env->CallObjectMethod(obj_File, mid_getPath);
    const char* path = env->GetStringUTFChars(obj_Path, nullptr);

    std::string routes(path);

    env->ReleaseStringUTFChars(obj_Path, path);
    return routes;
}

void init(JNIEnv *env, jclass clazz, jobject context) {
    Config config;
    // get external path where mods shall be located and store it on a global variable
    globvar::home_route = getConfigPath(env, context);
    //initConfig(&config,configPath);
    pthread_t ptid;
    pthread_create(&ptid, nullptr, Function_Hooker::hook_thread, nullptr);
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {
                    ("init"),
                    ("(Landroid/content/Context;)V"),
                    reinterpret_cast<void *>(init)
            },
    };
    jclass clazz = env->FindClass(("com/unity3d/player/UnityPlayerActivity"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (RegisterMain(env) != 0)
        return JNI_ERR;
    return JNI_VERSION_1_6;
}
