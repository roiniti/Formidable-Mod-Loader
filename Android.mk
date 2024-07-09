LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libluajit_armeabi_v7a
LOCAL_SRC_FILES := lib/armeabi-v7a/libluajit.a
LOCAL_MULTILIB := 32
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libluajit_arm64_v8a
LOCAL_SRC_FILES := lib/arm64-v8a/libluajit.a
LOCAL_MULTILIB := 64
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libluajit_x86
LOCAL_SRC_FILES := lib/x86/libluajit.a
LOCAL_MULTILIB := 32
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := FormidableML

LOCAL_CPPFLAGS := -w -std=c++17
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES += $(LOCAL_PATH) libluajit


ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_STATIC_LIBRARIES := libluajit_armeabi_v7a
else ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
    LOCAL_STATIC_LIBRARIES := libluajit_arm64_v8a
else ifeq ($(TARGET_ARCH_ABI), x86)
    LOCAL_STATIC_LIBRARIES := libluajit_x86
endif


LOCAL_SRC_FILES := Main.cpp \
	Substrate/hde64.c \
	Substrate/SubstrateDebug.cpp \
	Substrate/SubstrateHook.cpp \
	Substrate/SubstratePosixMemory.cpp \
	Substrate/SymbolFinder.cpp \
	And64InlineHook/And64InlineHook.cpp \
	core/hooker.cpp \
	core/my_cpparser.cpp \
	core/lua_hooker.cpp \
	core/hook_functions.cpp \
	core/variables.cpp \
	logger/clogger.cpp \
	ModApi/my_lua_tolstring.cpp \
	ModApi/ModReader.cpp \


include $(BUILD_SHARED_LIBRARY)
