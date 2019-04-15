//
// Created by yu.cai on 2019/4/15.
//

#include <jni.h>
#include "merge_mp4_util.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL merge(JNIEnv *env, jclass cls, jstring audiopath, jstring videopath, jstring outputpath) {
    char *pAudioPath = (char *)env->GetStringUTFChars(audiopath, NULL);
    char *pVideoPath = (char *)env->GetStringUTFChars(videopath, NULL);
    char *pOutputPath = (char *)env->GetStringUTFChars(outputpath, NULL);
    int ret =  MergeTwoFile(pAudioPath, pVideoPath, pOutputPath);
    env->ReleaseStringUTFChars(audiopath, pAudioPath);
    env->ReleaseStringUTFChars(videopath, pVideoPath);
    env->ReleaseStringUTFChars(outputpath, pOutputPath);
    return ret;
}

#ifdef __cplusplus
}
#endif

