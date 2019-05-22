//
// Created by yu.cai on 2019/4/15.
//
#ifndef JNI_HELPER_H
#define JNI_HELPER_H

#include <jni.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <memory>

#include <android/log.h>

//////////////////////////////////////////////////////////////////////////
#define TAG "JNI-HELPER"

#define CHECK(x, msg)                                                  \
  if (x) {                                                             \
  } else {                                                             \
    __android_log_print(ANDROID_LOG_ERROR, TAG, "%s:%d: %s", __FILE__, \
                        __LINE__, msg);                                \
    abort();                                                           \
  }

#define CHECK_EXCEPTION(jni, msg) \
  if (0) {                        \
  } else {                        \
    if (jni->ExceptionCheck()) {  \
      jni->ExceptionDescribe();   \
      jni->ExceptionClear();      \
      CHECK(0, msg);              \
    }                             \
  }

#define ARRAYSIZE(instance)                                     \
  static_cast<int>(sizeof(instance) / sizeof(instance[0]))

//引入在jni_load.cpp中定义的全局变量
extern JavaVM* g_vm;
//////////////////////////////////////////////////////////////////////////

class AttachThreadScoped
{
public:
    explicit AttachThreadScoped(JavaVM* jvm)
            : attached_(false), jvm_(jvm), env_(NULL)
    {
        jint ret_val = jvm->GetEnv(reinterpret_cast<void**>(&env_), JNI_VERSION_1_6);
        if (ret_val == JNI_EDETACHED)
        {
            ret_val = jvm_->AttachCurrentThread(&env_, NULL);
            attached_ = ret_val == JNI_OK;
            assert(attached_);
        }
    }
    ~AttachThreadScoped()
    {
        if (attached_ && (jvm_->DetachCurrentThread() < 0))
        {
            assert(false);
        }
    }
    JNIEnv* env()
    {
        return env_;
    }

private:
    bool attached_;
    JavaVM* jvm_;
    JNIEnv* env_;
};

//////////////////////////////////////////////////////////////////////////

class ClassReferenceHolder
{
public:
    ClassReferenceHolder(){}
    ClassReferenceHolder(const char** classes, int size)
    {
        AddClassReferences(classes, size);
    }
    void AddClassReferences(const char** classes, int size){
        AttachThreadScoped ats(g_vm);
        JNIEnv* env = ats.env();
        for (int i = 0; i < size; ++i)
        {
            LoadClass(env, classes[i]);
        }
    }
    void AddClassReference(const char* className){
        AttachThreadScoped ats(g_vm);
        JNIEnv* env = ats.env();
        LoadClass(env, className);
    }
    ~ClassReferenceHolder()
    {
        FreeReferences();
    }
    void FreeReferences()
    {
        AttachThreadScoped ats(g_vm);
        JNIEnv* env = ats.env();
        std::map<std::string, jclass>::const_iterator it;
        for (it = classes_.begin(); it != classes_.end(); ++it)
        {
            env->DeleteGlobalRef(it->second);
        }
        classes_.clear();
    }
    jclass GetClass(const std::string& name)
    {
        std::map<std::string, jclass>::iterator it = classes_.find(name);
        CHECK(it != classes_.end(), "Could not find class");
        return it->second;
    }

private:
    void LoadClass(JNIEnv* env, const std::string& name)
    {
        jclass localRef = env->FindClass(name.c_str());
        CHECK_EXCEPTION(env, "Could not load class");
        CHECK(localRef, name.c_str());
        jclass globalRef = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
        CHECK_EXCEPTION(env, "error during NewGlobalRef");
        CHECK(globalRef, name.c_str());
        bool inserted = classes_.insert(std::make_pair(name, globalRef)).second;
        CHECK(inserted, "Duplicate class name");
    }

    std::map<std::string, jclass> classes_;
};
//常用的jstring工具类，通过析构释放内存
class JString{
public:
    JString(JNIEnv* env, jstring str): env(env), str(str){
        pstr = (char*)env->GetStringUTFChars(str, NULL);
        len = env->GetStringLength(str);
    }
    ~JString(){
        env->ReleaseStringUTFChars(str, pstr);
    }
    const char* GetString() const {
        return pstr;
    }
    int StrLength() const {
        return len;
    }
private:
    JNIEnv* env;
    jstring str;
    char* pstr;
    int len;
};

#endif //JNI_HELPER_H
