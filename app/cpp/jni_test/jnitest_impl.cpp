//
// Created by yu.cai on 2019/4/16.
//

#include "./jni_helper.h"
#include "./reg_class_define.h"
#include <thread>
#ifdef __cplusplus
extern "C" {
#endif

extern std::shared_ptr<ClassReferenceHolder> g_class_reference_holder;

JNIEXPORT jint  JNICALL add(JNIEnv *env, jclass cls, jint a, jint b){
    jclass callback = g_class_reference_holder->GetClass(JNIREG_JNICALLBACE_CLASS);
    jmethodID methodId = env->GetStaticMethodID(callback, "add", "(II)I");
    if(methodId){
        return env->CallStaticIntMethod(callback, methodId, a, b);
    }
    return 0;
}
JNIEXPORT void JNICALL sayMessage(JNIEnv* env, jclass cls, jstring name, jstring message){
    JString NameStr(env, name);
    JString MessageStr(env, message);

    jclass callback = g_class_reference_holder->GetClass(JNIREG_JNICALLBACE_CLASS);
    jmethodID noParamInitFun = env->GetMethodID(callback, "<init>", "()V");
    jmethodID paramInitFun = env->GetMethodID(callback, "<init>", "(Ljava/lang/String;)V");
    jmethodID  print = env->GetMethodID(callback, "printf", "(Ljava/lang/String;)V");
    jmethodID  setParam = env->GetMethodID(callback, "setJniParam", "(Lcom/example/jni/JniParam;)Z");
    jobject o1 = env->NewObject(callback, noParamInitFun);
    env->CallVoidMethod(o1, print, message);
    env->DeleteLocalRef(o1);
    jobject o2 = env->NewObject(callback, paramInitFun, name);
    env->CallVoidMethod(o2, print, message);

    jclass jni_param_cls = g_class_reference_holder->GetClass(JNIREG_JNIPARAM_CLASS);
    noParamInitFun = env->GetMethodID(jni_param_cls, "<init>", "()V");
    jfieldID nameID = env->GetFieldID(jni_param_cls, "name", "Ljava/lang/String;");
    jfieldID schoolID = env->GetFieldID(jni_param_cls, "school", "Ljava/lang/String;");
    jfieldID mathID = env->GetFieldID(jni_param_cls, "math", "F");
    jfieldID chineseID = env->GetFieldID(jni_param_cls, "chinese", "F");
    jobject o3 = env->NewObject(jni_param_cls, noParamInitFun);
    env->SetObjectField(o3,nameID, env->NewStringUTF("caiyu"));
    env->SetObjectField(o3, schoolID, env->NewStringUTF("上海理工大学"));
    env->SetFloatField(o3, mathID, 99);
    env->SetFloatField(o3, chineseID, 80.5);
    env->CallBooleanMethod(o2, setParam, o3);

    env->DeleteLocalRef(o2);
    env->DeleteLocalRef(o3);

    //native thread call java's method
    std::thread t([](){

        jclass callback = g_class_reference_holder->GetClass(JNIREG_JNICALLBACE_CLASS);
        jclass jni_param_cls = g_class_reference_holder->GetClass(JNIREG_JNIPARAM_CLASS);

        AttachThreadScoped ats(g_vm);
        JNIEnv* env = ats.env();

        jmethodID noParamInitFun = env->GetMethodID(jni_param_cls, "<init>", "()V");
        jfieldID nameID = env->GetFieldID(jni_param_cls, "name", "Ljava/lang/String;");
        jfieldID schoolID = env->GetFieldID(jni_param_cls, "school", "Ljava/lang/String;");
        jfieldID mathID = env->GetFieldID(jni_param_cls, "math", "F");
        jfieldID chineseID = env->GetFieldID(jni_param_cls, "chinese", "F");
        jobject o3 = env->NewObject(jni_param_cls, noParamInitFun);
        env->SetObjectField(o3,nameID, env->NewStringUTF("dandan"));
        env->SetObjectField(o3, schoolID, env->NewStringUTF("上海理工大学"));
        env->SetFloatField(o3, mathID, 99);
        env->SetFloatField(o3, chineseID, 96);

        jmethodID threadCallback = env->GetStaticMethodID(callback, "threadCallback", "(Lcom/example/jni/JniParam;)V");

        if(threadCallback){
            env->CallStaticVoidMethod(callback, threadCallback,o3);
        }
        env->DeleteLocalRef(o3);

    });
    t.detach();
}

JNIEXPORT void JNICALL setJniParam(JNIEnv* env, jclass cls, jobject obj){
    jclass jni_param_cls = g_class_reference_holder->GetClass(JNIREG_JNIPARAM_CLASS);
    jfieldID nameID = env->GetFieldID(jni_param_cls, "name", "Ljava/lang/String;");
    jfieldID schoolID = env->GetFieldID(jni_param_cls, "school", "Ljava/lang/String;");
    jfieldID mathID = env->GetFieldID(jni_param_cls, "math", "F");
    jfieldID chineseID = env->GetFieldID(jni_param_cls, "chinese", "F");
    jstring name = (jstring)env->GetObjectField(obj, nameID);
    jstring school = (jstring)env->GetObjectField(obj, schoolID);
    float math = env->GetFloatField(obj, mathID);
    float chinese = env->GetFloatField(obj, chineseID);
}

#ifdef __cplusplus
}
#endif

