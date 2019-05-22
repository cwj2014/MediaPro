package com.example.jni;

public class JniTest {
    static {
        System.loadLibrary("JniTest");
    }
    public static native int add(int a, int b);
    public static native void sayMessage(String name, String message);
    public static native void setJniParam(JniParam param);
}
