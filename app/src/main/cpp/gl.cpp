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

    vec3f barycentric(vec3f a, vec3f b, vec3f c, vec3f p) {
        vec3f s[2];
        for (int i = 2; i--;) {
            s[i][0] = c[i] - a[i];
            s[i][1] = b[i] - a[i];
            s[i][2] = a[i] - p[i];
        }
        vec3f u = cross(s[0], s[1]);
        if (std::abs(u[2]) > 1e-2)
            return vec3f(1.0f - (u.x + u.y) / u.z, 1.0f * u.y / u.z, 1.0f * u.x / u.z);
        return vec3f(-1, 1, 1);
    }

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, TGAColor &color) {
        int width = image.get_width();
        int height = image.get_height();

        vec2 bmin(width - 1, height - 1);
        vec2 bmax(0, 0);
        vec2 clamp(width - 1, height - 1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                bmin[j] = std::max(std::min((int) u[i][j], bmin[j]), 0);
                bmax[j] = std::min(clamp[j], std::max(bmax[j], (int) u[i][j]));
            }
        }
        vec3f p;
        for (p.y = bmin.y; p.y <= bmax.y; ++p.y) {
            for (p.x = bmin.x; p.x <= bmax.x; ++p.x) {
                vec3f s = barycentric(u[0], u[1], u[2], p);
                if (s.x < 0 || s.y < 0 || s.z < 0)continue;
                p.z = 0;
                for (int i = 0; i < 3; i++) p.z += u[i][2] * s[i];
                if (zbuffer[(int) (p.x + p.y * width)] < p.z) {
                    image.set(p.x, p.y, color);
                    zbuffer[(int) (p.x + p.y * width)] = p.z;
                }
            }
        }
    }

    void triangle(vec3f *u, float *zbuffer, TGAImage &image, vec2f *texts, Model *model) {
        int width = image.get_width();
        int height = image.get_height();

        vec2 bmin(width - 1, height - 1);
        vec2 bmax(0, 0);
        vec2 clamp(width - 1, height - 1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                bmin[j] = std::max(std::min((int) u[i][j], bmin[j]), 0);
                bmax[j] = std::min(clamp[j], std::max(bmax[j], (int) u[i][j]));
            }
        }
        vec3f p;
        for (p.x = bmin.x; p.x <= bmax.x; ++p.x) {
            for (p.y = bmin.y; p.y <= bmax.y; ++p.y) {
                vec3f s = barycentric(u[0], u[1], u[2], p);
                if (s.x < 0 || s.y < 0 || s.z < 0)continue;
                p.z = 0;
                vec2f pText(0, 0);
                for (int i = 0; i < 3; i++) p.z += u[i][2] * s[i];
                for (int i = 0; i < 3; i++) pText[0] += texts[i][0] * s[i];
                for (int i = 0; i < 3; i++) pText[1] += texts[i][1] * s[i];
                if (zbuffer[(int) (p.x + p.y * width)] < p.z) {
                    TGAColor color = model->diffuse(pText);
                    image.set(p.x, p.y, color);
                    zbuffer[(int) (p.x + p.y * width)] = p.z;
                }
            }
        }
    }

    vec3f world2screen(vec3f v, int width, int height) {
        return vec3f(int((v.x + 1.) * width / 2. + 0.5), int((v.y + 1.) * height / 2. + 0.5), v.z);
    }
}