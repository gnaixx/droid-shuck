//
// Created by 薛祥清 on 2017/2/27.
//

#ifndef DROID_SHUCK_SHUCK_H
#define DROID_SHUCK_SHUCK_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include "Common.h"

#define REG_CLASSNAME "cc/gnaixx/droid_shuck/StubApplication"

void attachBaseContextNative(JNIEnv *, jobject, jobject);

int initClass(JNIEnv *, jobject, jobject);

int dFindClass(JNIEnv *, jclass *, const char *);

void onCreateNative(JNIEnv *, jobject);


#endif //DROID_SHUCK_SHUCK_H
