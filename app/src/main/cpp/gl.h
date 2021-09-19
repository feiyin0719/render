//
// Created by yinfei on 2021/9/14.
//
#include "tgaimage.h"
#include "geometry.h"

#ifndef RENDER_GL_H
namespace GL {
    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color);

    void triangle(vec3 *u, float *zbuffer, TGAImage &image, TGAColor &color);

    vec3 world2screen(vec3f v, int width, int height);
}
#define RENDER_GL_H

#endif //RENDER_GL_H
