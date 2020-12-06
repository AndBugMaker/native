#include <jni.h>
#include <string>
#include "MD5.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_nativwapp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_nativwapp_MainActivity_signParams(JNIEnv *env, jobject thiz, jobject params,
                                                   jstring secret) {
    // 用于拼接字符串的数组
    char buff[100] = {0};
    // 用于拼接字符串的“游标”指针
    char *pos = buff;

    if (params == NULL)
        return env->NewStringUTF(buff);

    // 获取HashMap类entrySet()方法ID
    jclass hashmapClass = env->FindClass("java/util/HashMap");
    jmethodID entrySetMID = env->GetMethodID(hashmapClass, "entrySet", "()Ljava/util/Set;");
    // 调用entrySet()方法获取Set对象
    jobject setObj = env->CallObjectMethod(params, entrySetMID);
    // 调用size()方法获取HashMap键值对数量
//  jmethodID sizeMID = env->GetMethodID(hashmapClass, "size", "()I");
//  jint size = env->CallIntMethod(hashMapInfo, sizeMID);

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
    // 用于读取“课程-分数”键值对，注意：内部类使用$符号表示
    jclass entryClass = env->FindClass("java/util/Map$Entry");
    jmethodID getKeyMID = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    jmethodID getValueMID = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

    // HashMap只能存放引用数据类型，不能存放int等基本数据类型
    // 使用Integer类的intValue()方法获取int数据
    jclass integerClass = env->FindClass("java/lang/Integer");
    jmethodID valueMID = env->GetMethodID(integerClass, "intValue", "()I");

    // 循环检测HashMap中是否还有数据
    while (env->CallBooleanMethod(iteratorObj, hasNextMID)) {
        // 读取一条数据
        jobject entryObj = env->CallObjectMethod(iteratorObj, nextMID);

        // 提取数据中key值：String类型课程名字
        jstring courseJS = (jstring) env->CallObjectMethod(entryObj, getKeyMID);
        if (courseJS == NULL)   // HashMap允许null类型
            continue;
        // jstring转C风格字符串
        const char *courseStr = env->GetStringUTFChars(courseJS, NULL);

        // 提取数据中value值：Integer类型分数，并转为int类型
        jobject scoreObj = env->CallObjectMethod(entryObj, getValueMID);
        if (scoreObj == NULL)
            continue;
        int score = (int) env->CallIntMethod(scoreObj, valueMID);

        // 拼接字符串，sprintf函数返回拼接字符个数
        int strLen = sprintf(pos, "%s: ", courseStr);
        pos += strLen;
        int numLen = sprintf(pos, "%d. ", score);
        pos += numLen;

        // 释放UTF字符串资源
        env->ReleaseStringUTFChars(courseJS, courseStr);
        // 释放JNI局部引用资源
        env->DeleteLocalRef(entryObj);
        env->DeleteLocalRef(courseJS);
        env->DeleteLocalRef(scoreObj);
    }

    // 释放JNI局部引用: jclass jobject
    env->DeleteLocalRef(hashmapClass);
    env->DeleteLocalRef(setObj);
    env->DeleteLocalRef(setClass);
    env->DeleteLocalRef(iteratorObj);
    env->DeleteLocalRef(iteratorClass);
    env->DeleteLocalRef(entryClass);
    env->DeleteLocalRef(integerClass);
env->js
    // 生成jstring字符串并返回
    return env->NewStringUTF(buff);

//    const char *originStr;
//    //将jstring转化成char *类型
//    originStr = env->GetStringUTFChars(str, false);
//    MD5 md5 = MD5(buff);
//    std::string md5Result = md5.hexdigest();
//    //将char *类型转化成jstring返回给Java层
//    return env->NewStringUTF(md5Result.c_str());
}