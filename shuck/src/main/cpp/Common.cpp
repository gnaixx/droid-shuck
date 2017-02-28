//
// Created by 薛祥清 on 2017/2/28.
//
#include "Common.h"
#include "../../../../../../../../../Library/android-sdk-macosx/ndk-bundle/platforms/android-21/arch-mips64/usr/include/sys/stat.h"

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

char *jstringToChar(JNIEnv *env, jstring jstr) {
    const char *cstr = env->GetStringUTFChars(jstr, NULL);
    int len = strlen(cstr);
    char *result = (char *) calloc(len + 1, sizeof(char));
    strcpy(result, cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    return result;
}


void dexLoadArt(JNIEnv *env, const char *dexPath) {

}


int dexLoadDvm(JNIEnv *env, const char *dexPath) {
    LOGD("Start dexLoadDvm()");
    void (*openDexFile)(const u4 *args, union JValue *pResult) = NULL;

    void *dldvm = dlopen(LIB_DVM, RTLD_LAZY); //获取libvm句柄
    JNINativeMethod *natvieMethods = (JNINativeMethod *) dlsym(dldvm, JNI_NATIVE_METHOD); //获取注册函数
    int isExist = lookup(natvieMethods, METHOD_NAME, METHOD_SIGNATURE, &openDexFile);
    if (isExist == 1) {
        LOGD("openDexFile method found !!!");
    } else {
        LOGD("openDexFile method does not found !!!");
    }

    int fp;
    struct stat statBuff = {0};
    fp = open(dexPath, O_RDONLY);
    if(fp == 0){
        LOGE("open %s failed !!!", dexPath);
        return 0;
    }
    int status = fstat(fp, &statBuff);
    if(status != 0){
        LOGE("fstat failed !!!");
    }

    int dexLen = statBuff.st_size;
    LOGD("dexLen: %d, st_blksize:%d", dexLen, statBuff.st_blksize);
    char *dexBytes = (char *) mmap(0, dexLen, PROT_WRITE | PROT_READ, MAP_ANONYMOUS, fp, 0);//可读写,匿名映射，映射区不与任何文件关联

    /*  dex 文件的解密 */

    /*结构体内最大成员对齐 sizeof(ArrayObject) = 24*/
    char *fileContent = (char *) malloc(sizeof(ArrayObject) + dexLen); //所有指针为4字节
    ArrayObject *fileContentObj = (ArrayObject *) fileContent;
    fileContentObj->length =  dexLen;
    memcpy(fileContentObj->contents, dexBytes, dexLen);
    //memcpy(fileContent + 16, dexBytes, dexLen); //因为按照最大字节对齐 4+4, 4+()
    LOGD("dexLen:%d", fileContentObj->length);

    //u4 args[] = {(u4) fileContentObj};
    u4* args = (u4*) &fileContentObj; // args => fileContent
    union JValue pResult;
    int cookie;
    if(openDexFile != NULL){
        openDexFile(args, &pResult);
    }else{
        cookie = -1;
    }
    cookie = (u8) pResult.l;
    LOGD("openDexFile cookie:%d", cookie);
    free(fileContent);
    munmap(dexBytes, dexLen);
    LOGD("End dexLoadDvm()");
    return cookie;
}

//查找是否包含 openDexFile 函数
int lookup(JNINativeMethod *methods, const char *name, const char *signature,
           void (**fnPtr)(const u4 *, union JValue *)) {

    if (methods == NULL) {
        LOGD("%s is NULL", JNI_NATIVE_METHOD);
        return 0;
    }
    int i = 0;
    while (methods[i].name != NULL) {
        if (strcmp(name, methods[i].name) == 0 && strcmp(signature, methods[i].signature) == 0) { //比较函数名 函数签名
            *fnPtr = (void (*)(const u4 *, union JValue *)) methods[i].fnPtr;
            LOGD("lookup index:%d, name:%s", i, name);
            return 1;
        }
        i++;
    }
}

void readAssets(JNIEnv *env, jobject application, const char *dexName, const char *dexPath) {
    LOGD("Start readAssets()");
    AAssetManager *mgr;
    if (access(dexPath, F_OK) != 0) {
        jclass Application = env->GetObjectClass(application);
        jmethodID Application_getAssets_mID = env->GetMethodID(Application, "getAssets",
                                                               "()Landroid/content/res/AssetManager;");
        jobject mgrjobj = env->CallObjectMethod(application, Application_getAssets_mID);

        mgr = AAssetManager_fromJava(env, mgrjobj);
        if (mgr == NULL) {
            LOGE("AAssetManager == NULL");
            return;
        }
        AAsset *asset = AAssetManager_open(mgr, dexName, AASSET_MODE_STREAMING);
        FILE *fp;
        void *buffer;
        int byteSize;
        if (asset) {
            fp = fopen(dexPath, "w");
            buffer = malloc(MAX_BUFFER_LEN);
            while (1) {
                byteSize = AAsset_read(asset, buffer, MAX_BUFFER_LEN);
                if (byteSize == 0) {
                    break;
                }
                fwrite(buffer, byteSize, 1, fp);
            }
            free(buffer);
            fclose(fp);
            AAsset_close(asset);
            LOGD("write %s success", dexPath);
        } else {
            LOGE("assets open failed !!!");
        }
    } else {
        LOGD("%s existed", dexPath);
    }
    LOGD("End readAssets()");
}