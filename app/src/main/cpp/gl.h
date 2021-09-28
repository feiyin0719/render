//
// Created by yinfei on 2021/9/14.
//
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

#ifndef RENDER_GL_H
#define RENDER_GL_H

namespace GL {

    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color);

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, TGAColor &color);

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, vec2f *texts, Model *model);

}
#endif //RENDER_GL_H
