#include <jni.h>
#include <string>
#include <math.h>
#include <android/bitmap.h>
#include "tgaimage.h"
#include "androidimage.h"
#include "gl.h"


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

    line(x0, y0, x1, y1, *androidImage.getImage(), tgaColor);
    androidImage.flush();

}

