//
// Created by yu.cai on 2019/4/16.
//

#include <jni.h>
#include "copy_assert_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean  JNICALL extractAssetFileToDataDir(JNIEnv *env, jclass cls, jobject obj, jstring assertFilePath, jstring dataSubDir) {
    char *pAssertFilePath = (char *) env->GetStringUTFChars(assertFilePath, NULL);
    char *pDataSubDir = (char*) env->GetStringUTFChars(dataSubDir, NULL);
    bool ret = CopyAssetsFile2DataDir(env, obj, std::string(pAssertFilePath), std::string(pDataSubDir));
    env->ReleaseStringUTFChars(assertFilePath, pAssertFilePath);
    env->ReleaseStringUTFChars(dataSubDir, pDataSubDir);
    return ret;
}

#ifdef __cplusplus
}
#endif

