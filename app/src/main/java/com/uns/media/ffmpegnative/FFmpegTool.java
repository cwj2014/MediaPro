package com.uns.media.ffmpegnative;

import java.util.Vector;

public class FFmpegTool extends LoadFFmpegHelper {
    static {
        System.loadLibrary("ffmpegtool");
    }

    /**
     * 执行结果监听器
     */
    public interface OnExecCmdResultListener{
        void onExecCmdResult(int ret);
    }

    static class ExecCmdThread extends Thread{
        private String _params[];
        private OnExecCmdResultListener _listener;
        public ExecCmdThread(String[] params, OnExecCmdResultListener listener){
            _params = params;
            _listener = listener;
        }

        @Override
        public void run() {
            final int ret = execCmd(_params);
            if(_listener != null){
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        _listener.onExecCmdResult(ret);
                    }
                });
            }
        }
    }
    /**
     * 获取ffmpeg版本
     * @return
     */
    public native static String getVersion();
    /**
     * 执行ffmpeg命令,耗时操作
     * @param params: 参数，参考ffmpeg可执行文件，eg: 截图 -ss 5 -i test.flv -f image2 -s 352X288 -y out.jpg
     * @return
     */
    private static native int execCmd(String[] params);
    private static void execCmd(Vector<String> params, final OnExecCmdResultListener listener){
        String[] arr = params.toArray(new String[params.size()]);
        ExecCmdThread execCmdThread = new ExecCmdThread(arr, listener);
        execCmdThread.start();
    }
    /**
     * 合成两个音视频mp4格式的文件,该函数是个耗时操作，请在子线程中执行
     * @param audioFile：音频输入源
     * @param videoFile：视频输入源
     * @param outputFile：合成输出文件
     * @return 0：成功，非0失败
     */
    public static native int merge(String audioFile, String videoFile, String outputFile);
    /**
     * 截取视频指定时间的图片，生成缩略图.jpeg
     * 格式：-ss 5 -i test.flv -f image2 -s 352X288 -y out.jpg
     * @param mediaFilePath: 输入文件
     * @param imageFilePath: 输出文件
     * @param time: 时间
     * @param width; 图片宽
     * @param height: 图片宽
     * @return
     */
     public static void captureImage(String mediaFilePath, String imageFilePath, long time, int width, int height, OnExecCmdResultListener listener) throws Exception {
         if(mediaFilePath.isEmpty()||imageFilePath.isEmpty())
             throw new Exception("参数不正确");
         Vector<String> params = new Vector<String>();
         if(time<=0){
             time  = 1;
         }
         params.add("-ss");
         params.add(String.valueOf(time));
         params.add("-i");
         params.add(mediaFilePath);
         if(width >0 && height >0){
            params.add("-s");
            params.add(String.format("%dx%d",width, height));
         }else if(width == -1 && height == -1){

         }else{
             throw new Exception("参数不正确");
         }
         params.add("-f");
         params.add("image2");
         params.add("-y");
         params.add(imageFilePath);

         execCmd(params, listener);
     }

    /**
     * 视频转码，有分辩率，码率、帧率, 特慢慎用
     * eg:-i %s -r 25 -b 200 -s 1080x720 %s
     * @param inputFilePath
     * @param outputFilePath
     * @param bitrate
     * @param fps
     * @param width
     * @param height
     */
     public static void transferVideo(String inputFilePath, String outputFilePath, int bitrate, int fps, int width, int height, OnExecCmdResultListener listener) throws Exception {
         if(inputFilePath.isEmpty()||outputFilePath.isEmpty())
             throw new Exception("参数不正确");
         Vector<String> params = new Vector<String>();
         params.add("-i");
         params.add(inputFilePath);
         if(fps > 0) {
             params.add("-r");
             params.add(String.valueOf(fps));
         }
         if(bitrate >0){
             params.add("-b");
             params.add(String.valueOf(bitrate));
         }
         if(width > 0 && height > 0){
             params.add("-s");
             params.add(String.format("%dx%d",width, height));
         }
         params.add(outputFilePath);
         execCmd(params, listener);
     }
}
