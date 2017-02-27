//
// Created by 薛祥清 on 2017/2/27.
//

#include "Shuck.h"

static JNINativeMethod gMethods[] = {
        {"attachBaseContext", "(Landroid/content/Context;)V", (void *) attachBaseContextNative},
        {"onCreate",          "()V",                          (void *) onCreateNative}
};

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Start JNI_OnLoad()");

    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    jclass clazz = env->FindClass(REG_CLASSNAME);
    if (clazz == NULL) {
        LOGE("Find %s failed !!!", REG_CLASSNAME);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, NELEM(gMethods)) != JNI_OK) {
        LOGE("Register natives failed !!!");
    }
    return JNI_VERSION_1_6;
}

jclass BuildVersion;
int sdk_int;
int isDalvik;

jclass ActivityThread;
jfieldID ActivityThread_mPackages_fID;
jfieldID ActivityThread_mBoundApplication_fID;
jfieldID ActivityThread_mInitialApplication_fID;
jfieldID ActivityThread_mAllApplications_fID;
jmethodID ActivityThread_currentActivityThread_mID;

jclass AppBindData;
jfieldID AppBindData_info_fID;

jclass ArrayMap;
jmethodID ArrayMap_get_mID;

jclass HashMap;
jmethodID HashMap_get_mID;

jclass ArrayList;
jmethodID ArrayList_size_mID;
jmethodID ArrayList_get_mID;
jmethodID ArrayList_set_mID;

jclass Context;
jmethodID Context_getPackageName;
jmethodID Context_getApplicationInfo;
jmethodID Context_getClassLoader;
jmethodID Context_getAssets;
jmethodID Context_getPackageResourePath;

jclass WeakReference;
jmethodID WeakReference_get_mID;

jclass LoadedApk;
jfieldID LoadedApk_mClassLoader_fID;
jfieldID LoadedApk_mApplication_fID;

jclass ApplicationInfo;
jfieldID ApplicationInfo_dataDir_fID;
jfieldID ApplicationInfo_nativeLibraryDir_fID;
jfieldID ApplicationInfo_sourceDir_fID;

jclass Application;
jmethodID Application_onCreate_mID;
jmethodID Application_attach_mID;

jclass ContextWrapper;
jmethodID ContextWrapper_attachBaseContext_mID;

jclass PathClassLoader;
jfieldID PathClassLoader_mDexs_fID;

jclass BaseDexClassLoader;
jfieldID BaseDexClassLoader_pathList_fID;

jclass DexPathList;
jfieldID DexPathList_dexElements_fID;

jclass Element;
jfieldID Element_dexFile_fID;
jfieldID Element_file_fID;

jclass File;
jmethodID File_getAbsolutePath_mID;

jclass DexFile;
jfieldID DexFile_mCookie_fID;
jmethodID DexFile_OpenDexFile_mID;

jclass ClassLoader;
jmethodID ClassLoader_loadClass_mID;

jclass System;
jmethodID System_getProperty_mID;

jclass SystemProperties;
jmethodID SystemProperties_get_mID;

void attachBaseContextNative(JNIEnv *env, jobject jobj, jobject ctx) {
    LOGI("Start attachBaseContextNative()");
    jobject appObj = jobj;
    initClass(env, jobj, ctx);
}

int initClass(JNIEnv *env, jobject appObj, jobject ctx) {
    LOGI("Start initClass()");
    //获取版本号
    if (!dFindClass(env, &BuildVersion, "android/os/Build$VERSION")) {
        return FAIL;
    }
    jfieldID fieldID = env->GetStaticFieldID(BuildVersion, "SDK_INT", "I");
    sdk_int = env->GetStaticIntField(BuildVersion, fieldID);
    LOGD("sdk_init is %d", sdk_int);

    //获取UI主线程
    if (!dFindClass(env, &ActivityThread, "android/app/ActivityThread")) {
        return FAIL;
    }
    if (sdk_int > 18) {//api>18 改为 HashMap -> ArrayMap
        ActivityThread_mPackages_fID = env->GetFieldID(ActivityThread, "mPackages", "Landroid/util/ArrayMap;");
        //ArrayMap
        if (!dFindClass(env, &ArrayMap, "android/util/ArrayMap")) {
            return FAIL;
        }
        ArrayMap_get_mID = env->GetMethodID(ArrayMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    } else{
        ActivityThread_mPackages_fID = env->GetFieldID(ActivityThread, "mPackages", "Landroid/util/HashMap;");
        //HashMap
        if (!dFindClass(env, &ArrayMap, "android/util/HashMap")) {
            return FAIL;
        }
        HashMap_get_mID = env->GetMethodID(ArrayMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    }
    //ActivityThread
    ActivityThread_mBoundApplication_fID = env->GetFieldID(ActivityThread, "mBoundApplication", "Landroid/app/ActivityThread$AppBindData;");
    ActivityThread_mInitialApplication_fID = env->GetFieldID(ActivityThread, "mInitialApplication", "Landroid/app/Application;");
    ActivityThread_mAllApplications_fID = env->GetFieldID(ActivityThread, "mAllApplications", "Ljava/util/ArrayList;");
    ActivityThread_currentActivityThread_mID = env->GetStaticMethodID(ActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");

    //ActivityThread$AppBindData
    if (!dFindClass(env, &AppBindData, "android/app/ActivityThread$AppBindData")) {
        return FAIL;
    }
    AppBindData_info_fID = env->GetFieldID(AppBindData, "info", "Landroid/app/LoadedApk;");

    //ArrayList
    if (!dFindClass(env, &ArrayList, "java/util/ArrayList")) {
        return FAIL;
    }
    ArrayList_size_mID = env->GetMethodID(ArrayList, "size", "()I");
    ArrayList_get_mID = env->GetMethodID(ArrayList, "get", "(I)Ljava/lang/Object;");
    ArrayList_set_mID = env->GetMethodID(ArrayList, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");

    //Context
    if (!dFindClass(env, &Context, "android/content/Context")) {
        return FAIL;
    }
    Context_getPackageName = env->GetMethodID(Context, "getPackageName", "()Ljava/lang/String;");
    Context_getApplicationInfo = env->GetMethodID(Context, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
    Context_getClassLoader = env->GetMethodID(Context, "getClassLoader", "()Ljava/lang/ClassLoader;");
    Context_getAssets = env->GetMethodID(Context, "getAssets", "()Landroid/content/res/AssetManager;");
    Context_getPackageResourePath = env->GetMethodID(Context, "getPackageResourcePath", "()Ljava/lang/String;");

    //WeakReference
    if (!dFindClass(env, &WeakReference, "java/lang/ref/WeakReference")) {
        return FAIL;
    }
    WeakReference_get_mID = env->GetMethodID(WeakReference, "get", "()Ljava/lang/Object;");

    //LoadedApk
    if (!dFindClass(env, &LoadedApk, "android/app/LoadedApk")) {
        return FAIL;
    }
    LoadedApk_mClassLoader_fID = env->GetFieldID(LoadedApk, "mClassLoader", "Ljava/lang/ClassLoader;");
    LoadedApk_mApplication_fID = env->GetFieldID(LoadedApk, "mApplication", "Landroid/app/Application;");

    //AppLicationInfo
    if (!dFindClass(env, &ApplicationInfo, "android/content/pm/ApplicationInfo")) {
        return FAIL;
    }
    ApplicationInfo_dataDir_fID = env->GetFieldID(ApplicationInfo, "dataDir", "Ljava/lang/String;");
    ApplicationInfo_nativeLibraryDir_fID = env->GetFieldID(ApplicationInfo, "nativeLibraryDir", "Ljava/lang/String;");
    ApplicationInfo_sourceDir_fID = env->GetFieldID(ApplicationInfo, "sourceDir", "Ljava/lang/String;");

    //Application
    if (!dFindClass(env, &Application, "android/app/Application")) {
        return FAIL;
    }
    Application_onCreate_mID = env->GetMethodID(Application, "onCreate", "()V");
    Application_attach_mID = env->GetMethodID(Application, "attach", "(Landroid/content/Context;)V");

    //ContextWrapper
    if (!dFindClass(env, &ContextWrapper, "android/content/ContextWrapper")) {
        return FAIL;
    }
    ContextWrapper_attachBaseContext_mID = env->GetMethodID(ContextWrapper, "attachBaseContext", "(Landroid/content/Context;)V");

    //PathClassLoader
    LOGD("PathClassLoader start");
    if (!dFindClass(env, &PathClassLoader, "dalvik/system/PathClassLoader")) {
        return FAIL;
    }

    if (sdk_int > 13) {
        //BaseDexClassLoader
        if (!dFindClass(env, &BaseDexClassLoader, "dalvik/system/BaseDexClassLoader")) {
            return FAIL;
        }
        BaseDexClassLoader_pathList_fID = env->GetFieldID(BaseDexClassLoader, "pathList", "Ldalvik/system/DexPathList;");
        //DexPathList
        if (!dFindClass(env, &DexPathList, "dalvik/system/DexPathList")) {
            return FAIL;
        }
        DexPathList_dexElements_fID = env->GetFieldID(DexPathList, "dexElements", "[Ldalvik/system/DexPathList$Element;");
        //DexPathList$Element
        if (!dFindClass(env, &Element, "dalvik/system/DexPathList$Element")) {
            return FAIL;
        }
        Element_dexFile_fID = env->GetFieldID(Element, "dexFile", "Ldalvik/system/DexFile;");

        if (sdk_int > 22) {//6.0
            Element_file_fID = env->GetFieldID(Element, "dir", "Ljava/io/File;");
        } else {
            Element_file_fID = env->GetFieldID(Element, "file", "Ljava/io/File;");
        }
    }else{
        PathClassLoader_mDexs_fID = env->GetFieldID(PathClassLoader, "mDexs", "[Ldalvik/system/DexFile;");
    }

    //File
    if (!dFindClass(env, &File, "java/io/File")) {
        return FAIL;
    }
    File_getAbsolutePath_mID = env->GetMethodID(File, "getAbsolutePath", "()Ljava/lang/String;");
    LOGD("PathClassLoader end");

    //DexFile
    if (!dFindClass(env, &DexFile, "dalvik/system/DexFile")) {
        return FAIL;
    }
    if (sdk_int > 22) {
        DexFile_mCookie_fID = env->GetFieldID(DexFile, "mCookie", "Ljava/lang/Object;");
        DexFile_OpenDexFile_mID = env->GetStaticMethodID(DexFile, "openDexFile", "(Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/Object;");
    } else if (sdk_int > 19) {
        DexFile_mCookie_fID = env->GetFieldID(DexFile, "mCookie", "J");
        DexFile_OpenDexFile_mID = env->GetStaticMethodID(DexFile, "openDexFile", "(Ljava/lang/String;Ljava/lang/String;I)J");
    } else {
        DexFile_mCookie_fID = env->GetFieldID(DexFile, "mCookie", "I");
        DexFile_OpenDexFile_mID = env->GetStaticMethodID(DexFile, "openDexFile", "(Ljava/lang/String;Ljava/lang/String;I)I");
    }

    //ClassLoader
    if (!dFindClass(env, &ClassLoader, "java/lang/ClassLoader")) {
        return FAIL;
    }
    //android 5+以上无法用findClass找到android.app.Application类
    ClassLoader_loadClass_mID = env->GetMethodID(ClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    //System
    LOGI("System start");
    if (!dFindClass(env, &System, "java/lang/System")) {
        return FAIL;
    }
    System_getProperty_mID = env->GetStaticMethodID(System, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");

    //SystemProperties
    LOGI("SystemProperties start");
    if (!dFindClass(env, &SystemProperties, "android/os/SystemProperties")) {
        return FAIL;
    }

    SystemProperties_get_mID = env->GetStaticMethodID(SystemProperties, "get", "(Ljava/lang/String;)Ljava/lang/String;");
    //获取 vm 类型
    jstring vmNameKey = env->NewStringUTF("java.vm.name");
    jstring jvmName = (jstring) env->CallStaticObjectMethod(System, System_getProperty_mID, vmNameKey);
    const char * vmName= env->GetStringUTFChars(jvmName, NULL);
    LOGI("vmName:%s", vmName);
    env->ReleaseStringUTFChars(jvmName, vmName);


    // persist.sys.dalvik.vm.lib
    // persist.sys.dalvik.vm.lib.2
    jstring vmLibKey1 = env->NewStringUTF("persist.sys.dalvik.vm.lib");
    jstring vmLibKey2 = env->NewStringUTF("persist.sys.dalvik.vm.lib.2");
    jstring jvmLib = (jstring) env->CallStaticObjectMethod(SystemProperties, SystemProperties_get_mID, vmLibKey1);
    if(jvmLib == NULL){
        jvmLib = (jstring) env->CallStaticObjectMethod(SystemProperties, SystemProperties_get_mID, vmLibKey2);
    }
    const char *vmLib = env->GetStringUTFChars(jvmLib, NULL);
    LOGI("vmLib:%s", vmLib);
    env->ReleaseStringUTFChars(jvmLib, vmLib);

    //获取 vm 版本
    jstring vmVersionKey = env->NewStringUTF("java.vm.version");
    jstring jvmVersion = (jstring) env->CallStaticObjectMethod(System, System_getProperty_mID, vmVersionKey);
    const char *vmVersion = env->GetStringUTFChars(jvmVersion, NULL);
    double vmVersionInt = atof(vmVersion);
    if (vmVersionInt > 2) {
        isDalvik = 0;
    } else {
        isDalvik = 1;
    }
    LOGI("vmVersion:%s, vmVersionInt:%f, isDalvik:%d", vmVersion, vmVersionInt, isDalvik);
    env->ReleaseStringUTFChars(jvmVersion, vmVersion);
    return SUCC;
}

int dFindClass(JNIEnv *env, jclass *ptr, const char *className) {
    LOGI("Start dFindClass : %s", className);
    jobject globalRef;
    jclass clazz = env->FindClass(className);
    if (clazz != NULL) {
        globalRef = env->NewGlobalRef(clazz); //创建全局变量
        *ptr = (jclass) globalRef;
        return SUCC;
    } else {
        LOGE("Find %s failed !!!", className);
        return FAIL;
    }
}

void onCreateNative(JNIEnv *env, jobject jobj) {
    LOGI("Start onCreateNative()");
}