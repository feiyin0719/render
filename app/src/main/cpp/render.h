//
// Created by yinfei on 2021/9/15.
//
#include "tgaimage.h"
#include "geometry.h"

#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H


class Render {
protected:
    TGAImage *image;

    Render();

    float *zbuffer;

    void init();

public:
    Render(int width, int height);

    virtual void line(int x0, int y0, int x1, int y1, TGAColor &color);

    virtual void triangle(vec3 *u, TGAColor &color);

    virtual void lock();

    virtual void unlock();

    virtual int getWidth() {
        return image->get_width();
    }

    virtual int getHeight() {
        return image->get_height();
    }

    ~Render();

};


#endif //RENDER_RENDER_H
