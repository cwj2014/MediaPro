package com.example.mediapro;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.jni.JniParam;
import com.example.jni.JniTest;
import com.uns.media.ffmpegnative.FFmpegTool;
import com.uns.util.NativeTool;

import java.io.File;
import java.io.FileInputStream;

public class MainActivity extends AppCompatActivity {

    private ImageView imageView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

       // int t = JniTest.add(5, 6);
        JniTest.sayMessage("caicai", "say hello");
        JniParam param = new JniParam();
        param.name = "dandan";
        param.school = "adfafa";
        param.chinese = 100;
        param.math = 80;
        JniTest.setJniParam(param);

        ((TextView)findViewById(R.id.tv_show_version)).setText(FFmpegTool.getVersion());
        imageView = (ImageView)findViewById(R.id.imageView);
            //sdcard/寂寞寂寞不好 - 曹格.mp4
        //tx1234.mp4
        //半壶纱 - 刘珂矣.mp4
        String dir = Environment.getExternalStorageDirectory().getAbsolutePath();
        final String inputFile = dir+"/半壶纱 - 刘珂矣.mp4";
        //final String outputFile = dir + "/半壶纱 - 蔡宇.mp4";
        final String outputFile = dir + "/test.jpeg";


        if(!NativeTool.extractAssetFileToDataDir(this, "timg.gif", "")){
            Toast.makeText(this, "extractAssetFileToDataDir failed", Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(this, "extractAssetFileToDataDir success", Toast.LENGTH_SHORT).show();
        }

        try {

//            FFmpegTool.merge(inputFile,  dir + "/寂寞寂寞不好 - 曹格.mp4", dir+"/merge.yuv");
            FFmpegTool.captureImage(inputFile, outputFile, 20, 352, 288, new FFmpegTool.OnExecCmdResultListener() {
                @Override
                public void onExecCmdResult(int ret) {
                    Toast.makeText(MainActivity.this, "cmd 执行完成", Toast.LENGTH_SHORT).show();
                    imageView.setImageBitmap(getBitmapFromPath(outputFile));
                }
            });
//            FFmpegTool.transferVideo(inputFile, outputFile, 1800, 25, 720, 480, new FFmpegTool.OnExecCmdResultListener() {
//                @Override
//                public void onExecCmdResult(int ret) {
//                    Toast.makeText(MainActivity.this, "cmd 执行完成", Toast.LENGTH_SHORT).show();
//                }
//            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Bitmap getBitmapFromPath(String path) {

        if (!new File(path).exists()) {
            System.err.println("getBitmapFromPath: file not exists");
            return null;
        }
        // Bitmap bitmap = Bitmap.createBitmap(1366, 768, Config.ARGB_8888);
        // Canvas canvas = new Canvas(bitmap);
        // Movie movie = Movie.decodeFile(path);
        // movie.draw(canvas, 0, 0);
        //
        // return bitmap;

        byte[] buf = new byte[1024 * 1024];// 1M
        Bitmap bitmap = null;

        try {

            FileInputStream fis = new FileInputStream(path);
            int len = fis.read(buf, 0, buf.length);
            bitmap = BitmapFactory.decodeByteArray(buf, 0, len);
            if (bitmap == null) {
                System.out.println("len= " + len);
                System.err
                        .println("path: " + path + "  could not be decode!!!");
            }
        } catch (Exception e) {
            e.printStackTrace();

        }

        return bitmap;
    }
}
