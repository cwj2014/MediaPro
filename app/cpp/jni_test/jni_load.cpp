//
// Created by yu.cai on 2019/4/15.
//

#include "./jni_helper.h"
#include "./reg_class_define.h"

//全局变量，使用说明extern JavaVM* g_vm, extern std::shared_ptr<ClassReferenceHolder> g_class_reference_holder
JavaVM* g_vm = NULL; //用于AttachCurrentThread 和 DetachCurrentThread
std::shared_ptr<ClassReferenceHolder> g_class_reference_holder = nullptr; //用于加载java类和方法



#ifdef __cplusplus
extern "C" {
#endif

extern JNIEXPORT jint  JNICALL add(JNIEnv*, jclass, jint, jint);
extern JNIEXPORT void JNICALL sayMessage(JNIEnv*, jclass, jstring, jstring);
extern JNIEXPORT void JNICALL setJniParam(JNIEnv*, jclass, jobject);

#ifdef __cplusplus
}
#endif
// Java和JNI函数的绑定表

static JNINativeMethod JNITEST_method_table[] = {
        {"add", "(II)I", (void*)add},
        {"sayMessage", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)sayMessage},
        {"setJniParam", "(Lcom/example/jni/JniParam;)V", (void*)setJniParam}
};

static const char* g_classes[] = { JNIREG_JNICALLBACE_CLASS, JNIREG_JNITEST_CLASS, JNIREG_JNIPARAM_CLASS};

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

    registerNativeMethods(env, JNIREG_JNITEST_CLASS, JNITEST_method_table,ARRAYSIZE(JNITEST_method_table));

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

