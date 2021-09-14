#include <jni.h>
#include <string>
#include <math.h>
#include <android/bitmap.h>
#include "tgaimage.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_iffly_render_Render_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_iffly_render_Render_line(JNIEnv *env, jobject obj, int x0, int y0, int x1, int y1,
                                  int color,
                                  jobject bitmap
) {
    AndroidBitmapInfo bitmapInfo;
    if (ANDROID_BITMAP_RESULT_SUCCESS != AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) {
        return;
    }
    void *bitmapValue;
    if (ANDROID_BITMAP_RESULT_SUCCESS !=
        AndroidBitmap_lockPixels(env, bitmap, &bitmapValue
        )) {
        return;
    }

    bool changeXY = false;
    if (abs(x1 - x0) < abs(y1 - y0)) {
        std::swap(x1, y1);
        std::swap(x0, y0);
        changeXY = true;
    }
    if (x0 > x1) {
        std::swap(x1, x0);
        std::swap(y1, y0);
    }

    int32_t *bitmapPixs = (int32_t *) bitmapValue;
    int w = bitmapInfo.width;
    for (int x = x0; x < x1; ++x) {
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 + (y1 - y0) * t;
        if (changeXY) {
            bitmapPixs[w * x + y] = color;
        } else {
            bitmapPixs[w * y + x] = color;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);

}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool changeXY = false;
    if (abs(x1 - x0) < abs(y1 - y0)) {
        std::swap(x1, y1);
        std::swap(x0, y0);
        changeXY = true;
    }
    if (x0 > x1) {
        std::swap(x1, x0);
        std::swap(y1, y0);
    }
    for (int x = x0; x < x1; ++x) {
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 + (y1 - y0) * t;
        if (changeXY) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}