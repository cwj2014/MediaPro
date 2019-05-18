package com.uns.util;

import android.content.Context;

public class NativeTool {
    static{
        System.loadLibrary("nativetool");
    }
    /**
     * 从asset目录复制文件至data目录下
     * @param ctx
     * @param assertFilePath
     * @param dataSubDir
     * @return
     */
    public native static boolean extractAssetFileToDataDir(Context ctx, String assertFilePath, String dataSubDir);
}
