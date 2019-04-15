//
// Created by yu.cai on 2019/4/15.
//
#include "com_uns_media_ffmpegnative_FFmpegTool.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

JNIEXPORT jstring JNICALL Java_com_uns_media_ffmpegnative_FFmpegTool_getVersion
  (JNIEnv *env, jclass jcls){
    char str[25];
    sprintf(str, "%d", avcodec_version());
    return env->NewStringUTF(str);
  }
