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
Java_com_iffly_render_Render_renderObject(JNIEnv *env, jobject thiz, jlong render,
                                          jstring file_name) {
    jboolean iscopy;
    const char *mfile = env->GetStringUTFChars(file_name, &iscopy);
    std::string data(mfile);
    env->ReleaseStringUTFChars(file_name, mfile);

    Model model(data);
    vec3f light(0, 0, -1);
    for (int i = 0; i < model.nfaces(); i++) {
        vec3f world_coords[3];
        vec3f screen_coords[3];
        vec2f texts[3];
        for (int j = 0; j < 3; j++) {
            world_coords[j] = model.vert(i, j);
            screen_coords[j] = GL::world2screen(world_coords[j], ((Render *) render)->getWidth(),
                                                ((Render *) render)->getHeight());
            texts[j] = model.uv(i, j);
        }
        ((Render *) render)->triangle(screen_coords, texts, &model);

//        vec3f norm = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
//        norm.normalize();
//        float intensity = light * norm;
//        if (intensity > 0) {
//            TGAColor color(255 * intensity, 255 * intensity, 255 * intensity, 255);
//            ((Render *) render)->triangle(screen_coords, color);
//        }
    }

}