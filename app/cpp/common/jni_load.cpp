//
// Created by yu.cai on 2019/4/15.
//

#include "../jni_helper.h"

//全局变量，使用说明extern JavaVM* g_vm, extern std::shared_ptr<ClassReferenceHolder> g_class_reference_holder
JavaVM* g_vm = NULL; //用于AttachCurrentThread 和 DetachCurrentThread
std::shared_ptr<ClassReferenceHolder> g_class_reference_holder = nullptr; //用于加载java类和方法


// 指定要注册的类，对应完整的java类名
#define JNIREG_NATIVETOOL_CLASS "com/uns/util/NativeTool"
#ifdef __cplusplus
extern "C" {
#endif

extern JNIEXPORT jboolean  JNICALL extractAssetFileToDataDir(JNIEnv *, jclass, jobject, jstring, jstring);

#ifdef __cplusplus
}
#endif
// Java和JNI函数的绑定表

static JNINativeMethod NativeTool_method_table[] = {
        {"extractAssetFileToDataDir", "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)Z", (void*)extractAssetFileToDataDir}
};

static const char* g_classes[] = { JNIREG_NATIVETOOL_CLASS};

// 注册native方法到java中
int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods);


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{

    g_vm = vm;
    int n = ARRAYSIZE(g_classes);
    g_class_reference_holder = std::make_shared<ClassReferenceHolder>(g_classes,n);

    AttachThreadScoped ats(g_vm);
    JNIEnv* env = ats.env();
    //映射函数

    registerNativeMethods(env, JNIREG_NATIVETOOL_CLASS, NativeTool_method_table,ARRAYSIZE(NativeTool_method_table));

    return JNI_VERSION_1_6;
}


int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz = g_class_reference_holder->GetClass(std::string(className));
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

