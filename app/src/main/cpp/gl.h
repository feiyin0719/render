//
// Created by yinfei on 2021/9/14.
//
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

#ifndef RENDER_GL_H
#define RENDER_GL_H

namespace GL {

    extern matf<4, 4> ModelView;
    extern matf<4, 4> Viewport;
    extern matf<4, 4> Projection;
    void viewport(const int x, const int y, const int w, const int h);

    void projection(
            const float coeff);

    void lookat(const vec3f eye, const vec3f center,
                const vec3f up);

    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color);

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, TGAColor &color);

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, vec2f *texts, Model *model);

    vec3f world2screen(vec3f v);
}
#endif //RENDER_GL_H
