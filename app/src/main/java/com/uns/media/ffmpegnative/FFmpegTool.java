package com.uns.media.ffmpegnative;

public class FFmpegTool extends LoadFFmpegHelper {
    static {
        System.loadLibrary("ffmpegtool");
    }

    /**
     * 获取ffmpeg版本
     * @return
     */
    public native static String getVersion();
    /**
     * 合成两个音视频mp4格式的文件,该函数是个耗时操作，请在子线程中执行
     * @param audioFile：音频输入源
     * @param videoFile：视频输入源
     * @param outputFile：合成输出文件
     * @return 0：成功，非0失败
     */
    public static native int merge(String audioFile, String videoFile, String outputFile);
}
