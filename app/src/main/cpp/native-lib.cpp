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
#include <android/log.h>

#define TAG "HELLO"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)
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
    vec2 u[3] = {vec2(x0, y0), vec2(x1, y1), vec2(x2, y2)};
    ((Render *) render)->triangle(u,
                                  tgaColor);
}

char *readFromAssets(JNIEnv *env, jobject assetManager, jstring filename) {

    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    if (mgr == NULL) {

        return nullptr;
    }


    /*获取文件名并打开*/
    jboolean iscopy;
    const char *mfile = env->GetStringUTFChars(filename, &iscopy);
    AAsset *asset = AAssetManager_open(mgr, mfile, AASSET_MODE_UNKNOWN);
    env->ReleaseStringUTFChars(filename, mfile);
    if (asset == NULL) {

        return nullptr;
    }
    /*获取文件大小*/
    off_t bufferSize = AAsset_getLength(asset);

    char *buffer = (char *) malloc(bufferSize + 1);
    buffer[bufferSize] = 0;
    int numBytesRead = AAsset_read(asset, buffer, bufferSize);
    AAsset_close(asset);
    return buffer;


}


extern "C"
JNIEXPORT void JNICALL
Java_com_iffly_render_Render_renderObject(JNIEnv *env, jobject thiz, jlong render,
                                          jobject asset_manager, jstring file_name) {
    char *buffer = readFromAssets(env, asset_manager, file_name);

    std::string data(buffer);
    free(buffer);

    Model model(data);

    for (int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        vec3f world_coords[3];
        vec2 screen_coords[3];
        for (int j = 0; j < 3; j++) {
            world_coords[j] = model.vert(face[j]);


            screen_coords[j] = GL::world2screen(world_coords[j], ((Render *) render)->getWidth(),
                                                ((Render *) render)->getHeight());
        }

        int r = std::rand() % 255;
        int g = std::rand() % 255;
        int b = std::rand() % 255;
        TGAColor color(r, g, b, 255);

        ((Render *) render)->triangle(screen_coords, color);
    }

}