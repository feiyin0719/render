//
// Created by yinfei on 2021/9/14.
//
#include "gl.h"

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool changeXY = false;
    if (abs(x1 - x0) < abs(y1 - y0)) {
        std::swap(x1, y1);
        std::swap(x0, y0);
        changeXY = true;
    }
    if (x0 > x1) {
        std::swap(x1, x0);
        std::swap(y1, y0);
    }
    int delta = (y1 > y0) ? 1 : -1;
    int error = 0;
    int dx = (x1 - x0);
    int derror1 = (y1 - y0) * delta * 2;
    int y = y0;
    for (int x = x0; x < x1; ++x) {
        if (changeXY) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error += derror1;
        if (error > dx) {
            y += delta;
            error -= dx * 2;
        }
    }
}