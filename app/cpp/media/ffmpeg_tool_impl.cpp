//
// Created by yu.cai on 2019/4/15.
//

#include <jni.h>
#include <cstdlib>
#include <malloc.h>
#include "ffmpeg_util.h"
#include <string.h>
#include <string>
#include <vector>
#include "ffmpeg_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL merge(JNIEnv *env, jclass cls, jstring audiopath, jstring videopath, jstring outputpath) {
    char *pAudioPath = (char *)env->GetStringUTFChars(audiopath, NULL);
    char *pVideoPath = (char *)env->GetStringUTFChars(videopath, NULL);
    char *pOutputPath = (char *)env->GetStringUTFChars(outputpath, NULL);
    //int ret =  MergeTwoFile(pAudioPath, pVideoPath, pOutputPath);
    int ret = DecodeVideoAndWriteYUV420(pAudioPath, pOutputPath);
    env->ReleaseStringUTFChars(audiopath, pAudioPath);
    env->ReleaseStringUTFChars(videopath, pVideoPath);
    env->ReleaseStringUTFChars(outputpath, pOutputPath);
    return ret;
}

JNIEXPORT jint JNICALL exec_ffmpeg_cmd(JNIEnv *env, jclass jcls, jobjectArray params){
    int argc = env->GetArrayLength(params);
    std::vector<std::string> vec;
    for (int i = 0; i < argc; i++) {
        jstring js = (jstring) env->GetObjectArrayElement(params, i);
        char* param = (char *)env->GetStringUTFChars(js, 0);
        vec.push_back(std::string(param));
        env->ReleaseStringUTFChars(js, param);
    }
    return executeCmd(vec);
}

#ifdef __cplusplus
}
#endif

