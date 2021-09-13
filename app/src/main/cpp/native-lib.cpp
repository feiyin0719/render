#include <jni.h>
#include <string>
#include <math.h>
#include "tgaimage.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_iffly_render_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
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
        int y = y0 + (y1 - y0 * t);
        if (changeXY) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}