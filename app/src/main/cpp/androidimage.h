//
// Created by yinfei on 2021/9/14.
//
#include <jni.h>
#include <math.h>
#include <android/bitmap.h>
#include "tgaimage.h"
#ifndef RENDER_ANDROIDIMAGE_H
#define RENDER_ANDROIDIMAGE_H

class AndroidImage : public TGAImage {
protected:
    JNIEnv *env;
    jobject bitmap;
    int32_t *bitmapPixs;
public:
    AndroidImage(JNIEnv *env, jobject bitmap);

    virtual void set(const int x, const int y, const TGAColor &c);

    virtual void unlockImage();

    virtual void lockImage();

};

#endif //RENDER_ANDROIDIMAGE_H

