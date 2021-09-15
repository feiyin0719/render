//
// Created by yinfei on 2021/9/14.
//
#include <jni.h>
#include <math.h>
#include <android/bitmap.h>
#include "tgaimage.h"

#ifndef RENDER_ANDROIDIMAGE_H
#define RENDER_ANDROIDIMAGE_H

class AndroidImage {
protected:
    TGAImage *image;
    AndroidBitmapInfo bitmapInfo;
    JNIEnv *env;
    jobject bitmap;
public:
    AndroidImage(JNIEnv *env, jobject bitmap);

    void flush();

    TGAImage *getImage() const;

    ~AndroidImage();
};


#endif //RENDER_ANDROIDIMAGE_H

