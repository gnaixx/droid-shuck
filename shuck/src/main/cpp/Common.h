//
// Created by 薛祥清 on 2017/2/27.
//

#ifndef DROID_SHUCK_COMMON_H
#define DROID_SHUCK_COMMON_H

#include <jni.h>
#include <android/log.h>


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


#endif //DROID_SHUCK_COMMON_H
