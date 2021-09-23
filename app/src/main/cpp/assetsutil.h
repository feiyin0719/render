//
// Created by yinfei on 2021/9/23.
//
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#ifndef RENDER_ASSETSUTIL_H
#define RENDER_ASSETSUTIL_H
namespace AssetsUtil {
    char *readFromAssets(JNIEnv *env, jobject assetManager, jstring filename);
}


#endif //RENDER_ASSETSUTIL_H
