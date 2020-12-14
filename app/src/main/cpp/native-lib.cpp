#include <jni.h>
#include <string>
#include "MD5.h"
#include <iostream>

using namespace std;
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_nativwapp_MainActivity_signParams(JNIEnv *env, jobject _, jobject params,
                                                   jstring secret) {
    string ret("");

    if (params == NULL)
        return env->NewStringUTF("");

    // 获取HashMap类entrySet()方法ID
    jclass hashmapClass = env->FindClass("java/util/Map");
    jmethodID entrySetMID = env->GetMethodID(hashmapClass, "entrySet", "()Ljava/util/Set;");
    // 调用entrySet()方法获取Set对象
    jobject setObj = env->CallObjectMethod(params, entrySetMID);

    // 获取Set类中iterator()方法ID
    jclass setClass = env->FindClass("java/util/Set");
    jmethodID iteratorMID = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    // 调用iterator()方法获取Iterator对象
    jobject iteratorObj = env->CallObjectMethod(setObj, iteratorMID);

    // 获取Iterator类中hasNext()方法ID
    // 用于while循环判断HashMap中是否还有数据
    jclass iteratorClass = env->FindClass("java/util/Iterator");
    jmethodID hasNextMID = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    // 获取Iterator类中next()方法ID
    // 用于读取HashMap中的每一条数据
    jmethodID nextMID = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

    // 获取Map.Entry类中getKey()和getValue()的方法ID
    // 用于读取“key-value”键值对，注意：内部类使用$符号表示
    jclass entryClass = env->FindClass("java/util/Map$Entry");
    jmethodID getKeyMID = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    jmethodID getValueMID = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

    // 循环检测HashMap中是否还有数据
    while (env->CallBooleanMethod(iteratorObj, hasNextMID)) {
        // 读取一条数据
        jobject entryObj = env->CallObjectMethod(iteratorObj, nextMID);

        // 提取数据中key值
        jstring keyJS = (jstring) env->CallObjectMethod(entryObj, getKeyMID);
        if (keyJS == NULL)   // HashMap允许null类型
            continue;
        // jstring转C风格字符串
        const char *keyStr = env->GetStringUTFChars(keyJS, NULL);

        // 提取数据中value值
        jstring valueJS = (jstring) env->CallObjectMethod(entryObj, getValueMID);
        if (valueJS == NULL)
            continue;
        const char *valueStr = env->GetStringUTFChars(valueJS, NULL);
        //拼接字符串
        ret.append(keyStr)
                .append("=")
                .append(valueStr)
                .append("&");


        // 释放UTF字符串资源
        env->ReleaseStringUTFChars(keyJS, keyStr);
        env->ReleaseStringUTFChars(valueJS, valueStr);
        // 释放JNI局部引用资源
        env->DeleteLocalRef(entryObj);
        env->DeleteLocalRef(keyJS);
        env->DeleteLocalRef(valueJS);
    }
    ret = ret.substr(0, ret.length() - 1).append(env->GetStringUTFChars(secret, NULL));
    // 释放JNI局部引用: jclass jobject
    env->DeleteLocalRef(hashmapClass);
    env->DeleteLocalRef(setObj);
    env->DeleteLocalRef(setClass);
    env->DeleteLocalRef(iteratorObj);
    env->DeleteLocalRef(iteratorClass);
    env->DeleteLocalRef(entryClass);
    MD5 md5 = MD5(ret);
    std::string md5Result = md5.hexdigest();
    //将char *类型转化成jstring返回给Java层
    return env->NewStringUTF(md5Result.c_str());
}