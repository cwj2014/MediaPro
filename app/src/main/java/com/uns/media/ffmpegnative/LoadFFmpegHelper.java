package com.uns.media.ffmpegnative;

import android.os.Handler;
import android.os.Looper;

abstract class LoadFFmpegHelper{
    static{
        //无加载顺序限制
        System.loadLibrary("avformat");
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avutil");
        System.loadLibrary("postproc");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
    }
    //用于子线程与主线程之间的UI交互
    protected static Handler handler = new Handler(Looper.getMainLooper());
}
