//
// Created by yinfei on 2021/9/23.
//

#include "assetsutil.h"
#include <jni.h>

namespace AssetsUtil {
    char *readFromAssets(JNIEnv *env, jobject assetManager, jstring filename) {
        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        if (mgr == NULL) {
            return nullptr;
        }
        /*获取文件名并打开*/
        jboolean iscopy;
        const char *mfile = env->GetStringUTFChars(filename, &iscopy);
        AAsset *asset = AAssetManager_open(mgr, mfile, AASSET_MODE_UNKNOWN);
        env->ReleaseStringUTFChars(filename, mfile);
        if (asset == NULL) {
            return nullptr;
        }
        /*获取文件大小*/
        off_t bufferSize = AAsset_getLength(asset);

        char *buffer = new char[bufferSize + 1];
        int numBytesRead = AAsset_read(asset, buffer, bufferSize);
        buffer[bufferSize] = 0;
        AAsset_close(asset);
        return buffer;
    }
}