//
// Created by yu.cai on 2019/4/15.
//

#ifndef MEDIAPRO_COYP_ASSERT_HELPER_H
#define MEDIAPRO_COYP_ASSERT_HELPER_H

#include <jni.h>
#include <android/log.h>
#include <vector>
#include <string>

#define  COPY_FILE_TAG    "copyassets"
#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,COPY_FILE_TAG,__VA_ARGS__)
#endif
#ifndef LOGE
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,COPY_FILE_TAG,__VA_ARGS__)
#endif


#include <unistd.h>
#include <stdio.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

static bool isFileExist(const char* path)
{
    int access_result = access(path, F_OK);
    LOGE("access_result=%d",access_result);
    return (access_result!=-1);
}
/**
 * 拷贝文件至data路径
 * @param mgr
 * @param assetFileName
 * @param toFilePath
 * @return
 */
static bool CopyAssetFile(AAssetManager* mgr,const char* assetFileName,const char* toFilePath)
{
    bool ret = false;
    AAsset* asset = nullptr;
    do{
        asset = AAssetManager_open(mgr, assetFileName,AASSET_MODE_UNKNOWN);
        if(asset==nullptr){
            LOGE(" %s","asset==NULL");
            break;
        }
        FILE * file = fopen(toFilePath,"w+");
        if(file==nullptr){
            LOGE("fopen error");
            break;
        }
        off_t bufferSize = AAsset_getLength(asset);
        char *buffer=(char *)malloc(bufferSize);
        off_t numBytesRead = AAsset_read(asset, buffer, bufferSize);
        fwrite(buffer,1,numBytesRead,file);
        free(buffer);
        fclose(file);
        ret = true;
    }while(0);
    if(asset != nullptr){
        AAsset_close(asset);
    }
    return ret;
}

class AssertManager{
public:
    AssertManager(JNIEnv * env,jobject& context):_env(env){
        contextWrapper = env->FindClass("android/content/ContextWrapper");
        jmethodID getAssets = env->GetMethodID(contextWrapper, "getAssets", "()Landroid/content/res/AssetManager;");
        assetManager = env->CallObjectMethod(context,getAssets);
        mgr = AAssetManager_fromJava(env, assetManager);
    }
    ~AssertManager(){
        _env->DeleteLocalRef(contextWrapper);
        _env->DeleteLocalRef(assetManager);
    }
public:
    AAssetManager* mgr;
private:
    JNIEnv* _env;
    jclass contextWrapper;
    jobject assetManager;
};

/**
 * 获取data根目录
 * @param env
 * @param context
 * @param dirPath
 * @return
 */
bool GetApkDataDir(JNIEnv * env,jobject& context, std::string& dirPath)
{
    jclass contextWrapper = env->FindClass("android/content/ContextWrapper");
    jmethodID method = env->GetMethodID(contextWrapper, "getFilesDir", "()Ljava/io/File;");
    jobject file = env->CallObjectMethod(context, method);
    jclass FileClass = env->GetObjectClass(file);
    jmethodID getAbsolutePath = env->GetMethodID(FileClass, "getAbsolutePath", "()Ljava/lang/String;");
    jobject dir = env->CallObjectMethod(file,getAbsolutePath);
    char * path = (char*)env->GetStringUTFChars((jstring)dir, NULL);
    bool ret = false;
    if(path!=nullptr){
        dirPath = std::string(path);
        ret = true;
    }
    env->DeleteLocalRef(dir);
    env->DeleteLocalRef(FileClass);
    env->DeleteLocalRef(file);
    env->DeleteLocalRef(contextWrapper);
    return ret;
}


static bool CopyAssetsFile2DataDir(AAssetManager* mgr, std::string assetFilePath, std::string dataRootDir, std::string subDir){
    std::string toFilePath = dataRootDir + std::string("/");
    if(!subDir.empty()){
        toFilePath += subDir + std::string("/");
    }
    std::string fileName = assetFilePath.substr(assetFilePath.find_last_of('/')+1);
    toFilePath += fileName;
    if(!isFileExist(toFilePath.c_str())){
        return CopyAssetFile(mgr,assetFilePath.c_str(),toFilePath.c_str());
    }
    return true;
}

/**
 * 拷贝到指定目录下
 * @param env
 * @param context
 * @param assetFilePath
 * @param toDir
 * @return
 */
bool CopyAssetsFile2DataDir(JNIEnv * env,jobject& context,std::string assetFilePath, std::string toDir)
{
    std::string dirPath;
    if(!GetApkDataDir(env,context,dirPath))
        return false;
    bool ret = false;
    do{
        std::shared_ptr<AssertManager> mgr = std::make_shared<AssertManager>(env, context);
        if(mgr==NULL)
        {
            LOGE(" %s","AAssetManager==NULL");
            break;
        }
        ret = CopyAssetsFile2DataDir(mgr->mgr, assetFilePath, dirPath, toDir);
    }while(0);

    return ret;
}
/**
 * 按asset目录结构拷贝
 * @param env
 * @param context
 * @param assetFilePath
 * @return
 */
bool CopyAssetsFile2DataDir(JNIEnv * env,jobject& context,std::string assetFilePath){
    int index = assetFilePath.find_last_of('/');
    std::string toDir = "";
    if(index != -1){
        toDir = assetFilePath.substr(0,index);
    }
    return CopyAssetsFile2DataDir(env, context, assetFilePath, toDir);
}

bool CopyAssetsFile2DataDir(JNIEnv * env,jobject& context,std::vector<std::string> assetFileNames)
{
    std::string dirPath;
    if(!GetApkDataDir(env,context,dirPath))
        return false;

    bool ret = false;
    do{
        std::shared_ptr<AssertManager> mgr = std::make_shared<AssertManager>(env, context);
        if(mgr==NULL)
        {
            LOGE(" %s","AAssetManager==NULL");
            break;
        }
        const char * path = dirPath.c_str();

        for(auto iter=assetFileNames.begin(); iter!=assetFileNames.end(); iter++){

            int index = (*iter).find_last_of('/');
            std::string toDir = "";
            if(index != -1){
                toDir = (*iter).substr(0,index);
            }
            if(!CopyAssetsFile2DataDir(mgr->mgr, *iter, dirPath, toDir)){
                break;
            }
        }
        ret = true;
    }while(0);
    return ret;
}


#endif //MEDIAPRO_COYP_ASSERT_HELPER_H
