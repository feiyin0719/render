//
// Created by yinfei on 2021/9/14.
//
#include "androidimage.h"

AndroidImage::AndroidImage(JNIEnv *env, jobject bitmap) {
    this->env = env;
    this->bitmap = env->NewGlobalRef(bitmap);
    if (ANDROID_BITMAP_RESULT_SUCCESS != AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) {
        throw std::bad_alloc();
    }
    this->width = bitmapInfo.width;
    this->height = bitmapInfo.height;
    this->bytespp = 4;
    this->bitmapPixs = nullptr;
}

void AndroidImage::lock() {
    void *bitmapValue;
    if (ANDROID_BITMAP_RESULT_SUCCESS !=
        AndroidBitmap_lockPixels(env, bitmap, &bitmapValue
        )) {
        throw std::logic_error("get bitmap value fail");
    }

    bitmapPixs = (int32_t *) bitmapValue;
}

void AndroidImage::unlock() {
    AndroidBitmap_unlockPixels(env, bitmap);
}

void AndroidImage::set(const int x, const int y, const TGAColor &c) {
    if (this->bitmapPixs != nullptr) {
        int width = bitmapInfo.width;
        bitmapPixs[y * width + x] = (int32_t) c.getLongColor();
    }
}

TGAColor AndroidImage::get(const int x, const int y) const {
    if (this->bitmapPixs != nullptr) {
        return TGAColor((long) bitmapPixs[y * width + x]);
    }
    return TGAColor();
}

void AndroidImage::clear() {
    if (this->bitmapPixs != nullptr) {
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
                bitmapPixs[y * width + x] = 0xffffffff;
    }
}