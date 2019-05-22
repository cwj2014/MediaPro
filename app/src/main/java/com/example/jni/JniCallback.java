package com.example.jni;

import android.util.Log;

/**
 * native层回调java层方法
 */
public class JniCallback {
    private String TAG = "JniCallback";
    private String name = "cc";
    public JniCallback(){
    }
    public JniCallback(String name){
       this.name = name;
    }
    public static int add(int a, int b){
        return a + b;
    }

    public void printf(String msg){
        Log.d(TAG, name + " : "+ msg);
    }

    public boolean setJniParam(JniParam param){
        return true;
    }

    public static void threadCallback(JniParam param){
        System.out.println(param.chinese);
    }
}
