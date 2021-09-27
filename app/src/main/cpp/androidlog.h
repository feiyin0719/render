//
// Created by yinfei on 2021/9/24.
//
#include <android/log.h>
#ifndef RENDER_ANDROIDLOG_H
#define RENDER_ANDROIDLOG_H


#define TAG "RENDER"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)
#endif //RENDER_ANDROIDLOG_H
