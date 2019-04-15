package com.uns.media.ffmpegnative;

public class FFmpegTool extends LoadFFmpegHelper {
    static {
        System.loadLibrary("ffmpegtool");
    }
    public native static String getVersion();
}
