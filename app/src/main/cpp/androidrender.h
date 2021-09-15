//
// Created by yinfei on 2021/9/15.
//
#include "render.h"
#include "androidimage.h"

#ifndef RENDER_ANDROIDRENDER_H
#define RENDER_ANDROIDRENDER_H


class AndroidRender : public Render {

public:
    AndroidRender(JNIEnv *env, jobject bitmap);


};


#endif //RENDER_ANDROIDRENDER_H
