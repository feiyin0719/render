//
// Created by yinfei on 2021/9/14.
//
#include "gl.h"
#include <iostream>
#include <cstdlib>

namespace GL {
    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color) {
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
        int derror = (y1 - y0) * delta * 2;
        int y = y0;
        for (int x = x0; x < x1; ++x) {
            if (changeXY) {
                image.set(y, x, color);
            } else {
                image.set(x, y, color);
            }
            error += derror;
            if (error > dx) {
                y += delta;
                error -= dx * 2;
            }
        }
    }

    vec3f barycentric(vec2 a, vec2 b, vec2 c, vec2 p) {
        vec3 s[2];
        for (int i = 2; i--;) {
            s[i][0] = c[i] - a[i];
            s[i][1] = b[i] - a[i];
            s[i][2] = a[i] - p[i];
        }
        vec3 u = cross(s[0], s[1]);
        if (u[2] !=
            0)
            return vec3f(1.0f - (u.x + u.y) / u.z, 1.0f * u.y / u.z, 1.0f * u.x / u.z);
        return vec3f(-1, 1, 1);
    }

    void triangle(vec2 *u, TGAImage &image, TGAColor &color) {
        vec2 bmin(image.get_width() - 1, image.get_height() - 1);
        vec2 bmax(0, 0);
        vec2 clamp(image.get_width() - 1, image.get_height() - 1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                bmin[j] = std::max(std::min(u[i][j], bmin[j]), 0);
                bmax[j] = std::min(clamp[j], std::max(bmax[j], u[i][j]));
            }
        }
        vec2 p;
        for (p.y = bmin.y; p.y <= bmax.y; ++p.y) {
            for (p.x = bmin.x; p.x <= bmax.x; ++p.x) {
                vec3f s = barycentric(u[0], u[1], u[2], p);
                if (s.x < 0 || s.y < 0 || s.z < 0)continue;
                image.set(p.x, p.y, color);
            }
        }
    }

    vec2 world2screen(vec3f v, int width, int height) {
        return vec2(int((v.x + 1.) * width / 2.), int((v.y + 1.) * height / 2.));
    }
}