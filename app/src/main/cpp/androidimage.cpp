//
// Created by yinfei on 2021/9/14.
//
#include "androidimage.h"

AndroidImage::AndroidImage(JNIEnv *env, jobject bitmap) {
    this->env = env;
    this->bitmap = bitmap;
    if (ANDROID_BITMAP_RESULT_SUCCESS != AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) {
        throw std::bad_alloc();
    }
    image = new TGAImage(bitmapInfo.width, bitmapInfo.height, 4);
}

void AndroidImage::flush() {
    void *bitmapValue;
    if (ANDROID_BITMAP_RESULT_SUCCESS !=
        AndroidBitmap_lockPixels(env, bitmap, &bitmapValue
        )) {
        throw std::logic_error("get bitmap value fail");
    }
    int width = bitmapInfo.width;
    int height = bitmapInfo.height;
    int32_t *bitmapPixs = (int32_t *) bitmapValue;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TGAColor c = image->get(x, y);
            bitmapPixs[y * width + x] =
                    c.bgra[3] << 24 | c.bgra[2] | c.bgra[1] << 8 | c.bgra[0] << 16;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

TGAImage *AndroidImage::getImage() const {
    return image;
}

AndroidImage::~AndroidImage() {
    delete image;
}