#include <jni.h>
#include <string>
#include <math.h>
#include <android/bitmap.h>
#include "tgaimage.h"
#include "androidimage.h"

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
    image.lockImage();
    for (int x = x0; x < x1; ++x) {
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 + (y1 - y0) * t;
        if (changeXY) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
    image.unlockImage();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_iffly_render_Render_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_iffly_render_Render_line(JNIEnv *env, jobject obj, int x0, int y0, int x1, int y1,
                                  jlong color,
                                  jobject bitmap
) {
    AndroidImage androidImage(env, bitmap);
    TGAColor tgaColor(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff,
                      color >> 24 & 0xff);
    line(x0, y0, x1, y1, androidImage, tgaColor);

}

