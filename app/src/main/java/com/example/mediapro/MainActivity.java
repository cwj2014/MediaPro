package com.example.mediapro;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import com.uns.media.ffmpegnative.FFmpegTool;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ((TextView)findViewById(R.id.tv_show_version)).setText(FFmpegTool.getVersion());
            //sdcard/寂寞寂寞不好 - 曹格.mp4
        String dir = Environment.getExternalStorageDirectory().getAbsolutePath();
        final String inputFile = dir+"/半壶纱 - 刘珂矣.mp4";
        final String outputFile = dir + "/半壶纱 - 蔡宇.mp4";

        try {
//            FFmpegTool.captureImage(inputFile, outputFile, 20, 352, 288, new FFmpegTool.OnExecCmdResultListener() {
//                @Override
//                public void onExecCmdResult(int ret) {
//                    Toast.makeText(MainActivity.this, "cmd 执行完成", Toast.LENGTH_SHORT).show();
//                }
//            });
            FFmpegTool.transferVideo(inputFile, outputFile, 1800, 25, 720, 480, new FFmpegTool.OnExecCmdResultListener() {
                @Override
                public void onExecCmdResult(int ret) {
                    Toast.makeText(MainActivity.this, "cmd 执行完成", Toast.LENGTH_SHORT).show();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
