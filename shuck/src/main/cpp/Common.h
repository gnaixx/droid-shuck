//
// Created by 薛祥清 on 2017/2/27.
//

#ifndef DROID_SHUCK_COMMON_H
#define DROID_SHUCK_COMMON_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


//日志
#define TAG "SHUCK_NDK"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

//计算 JNINativeMethod 数组大小
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

//success or failed
#define SUCC 1
#define FAIL 0

//数组大小
#define MAX_NAME_LEN 256
#define MAX_BUFFER_LEN 1024

#define HAVE_LITTLE_ENDIAN //直接定义小端编码



//dexLoadDvm
#define LIB_DVM "libdvm.so"
#define JNI_NATIVE_METHOD "dvm_dalvik_system_DexFile"
#define METHOD_NAME "openDexFile"
#define METHOD_SIGNATURE "([B)I"

typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;

union JValue {
#if defined(HAVE_LITTLE_ENDIAN)
    u1      z;
    s1      b;
    u2      c;
    s2      s;
    s4      i;
    s8      j;
    float   f;
    double  d;
    void*   l;
#endif
#if defined(HAVE_BIG_ENDIAN)
    struct {
        u1  _z[3];
        u1  z;
    };
    struct {
        s1  _b[3];
        s1  b;
    };
    struct {
        u2  _c;
        u2  c;
    };
    struct {
        s2  _s;
        s2  s;
    };
    s4      i;
    s8      j;
    float   f;
    double  d;
    void*   l;
#endif
};

struct Object {
    //ClassObject*    clazz;
    void *          clazz;
    u4              lock;
};

struct ArrayObject : Object {
    u4              length;
    u8              contents[1];
};

int dFindClass(JNIEnv *, jclass *, const char *);

char *jstringToChar(JNIEnv *, jstring);

int dexLoadDvm(JNIEnv *, const char *);

void dexLoadArt(JNIEnv *, const char *);

void readAssets(JNIEnv *, jobject, const char *, const char *);

int lookup(JNINativeMethod *, const char *, const char *, void (**)(const u4*, union JValue*));



#endif //DROID_SHUCK_COMMON_H
