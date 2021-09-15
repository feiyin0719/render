//
// Created by yinfei on 2021/9/15.
//
#include "tgaimage.h"

#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H


class Render {
protected:
    TGAImage *image;

    Render();

public:
    Render(int width, int height);

    virtual void line(int x0, int y0, int x1, int y1, TGAColor color);

    virtual void lock();

    virtual void unlock();

    ~Render();

};


#endif //RENDER_RENDER_H
