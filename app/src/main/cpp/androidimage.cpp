//
// Created by yinfei on 2021/9/14.
//
#include "androidimage.h"

AndroidImage::AndroidImage(JNIEnv *env, jobject bitmap) {
    this->env = env;
    this->bitmap = bitmap;
    AndroidBitmapInfo bitmapInfo;
    if (ANDROID_BITMAP_RESULT_SUCCESS != AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) {
        return;
    }
    width = bitmapInfo.width;
    height = bitmapInfo.height;
    bitmapPixs = nullptr;

}


void AndroidImage::set(int x, int y, const TGAColor &c) {
    if (bitmapPixs != nullptr) {
        bitmapPixs[y * width + x] = c.bgra[3] << 24 | c.bgra[2] | c.bgra[1] << 8 | c.bgra[0] << 16;
    }
}

void AndroidImage::lockImage() {
    void *bitmapValue;
    if (ANDROID_BITMAP_RESULT_SUCCESS !=
        AndroidBitmap_lockPixels(env, bitmap, &bitmapValue
        )) {
        return;
    }
    bitmapPixs = (int32_t *) bitmapValue;
}

void AndroidImage::unlockImage() {
    bitmapPixs = nullptr;
    AndroidBitmap_unlockPixels(env, bitmap);
}