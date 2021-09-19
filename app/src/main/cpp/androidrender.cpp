//
// Created by yinfei on 2021/9/15.
//

#include "androidrender.h"

AndroidRender::AndroidRender(JNIEnv *env, jobject bitmap) : Render() {

    image = new AndroidImage(env, bitmap);
    init();
}

