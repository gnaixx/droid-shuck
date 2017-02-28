//
// Created by 薛祥清 on 2017/2/27.
//

#ifndef DROID_SHUCK_SHUCK_H
#define DROID_SHUCK_SHUCK_H

#include "Common.h"

#define REG_CLASSNAME "cc/gnaixx/droid_shuck/StubApplication"
#define DEX_NAME "shuck.dex"


void attachBaseContextNative(JNIEnv *, jobject, jobject);

int initClass(JNIEnv *, jobject, jobject);

void replaceCookie(JNIEnv *, jobject);

void onCreateNative(JNIEnv *, jobject);


#endif //DROID_SHUCK_SHUCK_H
