package com.uns.nativeload;

public abstract class JniLoadHelper {
    static {
        System.loadLibrary("JNILoad");
    }
}
