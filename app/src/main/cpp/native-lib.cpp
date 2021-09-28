#include <jni.h>
#include <string>
#include <math.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "tgaimage.h"
#include "androidimage.h"
#include "gl.h"
#include "render.h"
#include "androidrender.h"
#include "model.h"
#include "assetsutil.h"
#include "androidlog.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_iffly_render_Render_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



extern "C" JNIEXPORT void JNICALL
Java_com_iffly_render_Render_line(JNIEnv *env, jobject obj, jlong render, int x0, int y0, int x1,
                                  int y1,
                                  jlong color

) {
    TGAColor tgaColor((long) color);
    ((Render *) render)->line(x0, y0, x1, y1, tgaColor);
}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_iffly_render_Render_init(JNIEnv *env, jobject thiz, jobject bitmap) {
    return (jlong) new AndroidRender(env, bitmap);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_lock(JNIEnv *env, jobject thiz, jlong render) {
    ((Render *) render)->lock();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_unlock(JNIEnv *env, jobject thiz, jlong render) {
    ((Render *) render)->unlock();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_destroy(JNIEnv *env, jobject thiz, jlong render) {
    delete ((Render *) render);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_triangle(JNIEnv *env, jobject thiz, jlong render, jint x0,
                                      jint y0, jint x1, jint y1, jint x2, jint y2, jlong color) {
    TGAColor tgaColor((long) color);
    vec3f u[3] = {vec3f(x0, y0, 0), vec3f(x1, y1, 0), vec3f(x2, y2, 0)};
    ((Render *) render)->triangle(u,
                                  tgaColor);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_renderObject(JNIEnv *env, jobject thiz, jlong render) {

    int width = ((Render *) render)->getWidth();
    int height = ((Render *) render)->getHeight();
    const vec3f light_dir(1, 1, 1); // light source
    const vec3f eye(1, 1, 3); // camera position
    const vec3f center(0, 0, 0); // camera direction
    const vec3f up(0, 1, 0); // camera up vector
    GL::lookat(eye, center, up);                            // build the ModelView matrix
    GL::viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4); // build the Viewport matrix
    GL::projection(-1.f / (eye - center).norm());               // build the Projection matrix

    ((Render *) render)->renderModels();


}
extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_loadModel(JNIEnv *env, jobject thiz, jlong render, jstring file_name) {
    jboolean iscopy;
    const char *mfile = env->GetStringUTFChars(file_name, &iscopy);
    std::string fileName(mfile);
    env->ReleaseStringUTFChars(file_name, mfile);
    ((Render *) render)->addModel(fileName);

}