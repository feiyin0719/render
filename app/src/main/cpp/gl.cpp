//
// Created by yinfei on 2021/9/14.
//
#include "gl.h"
#include <iostream>
#include <cstdlib>

namespace GL {
    matf<4, 4> ModelView;
    matf<4, 4> Viewport;
    matf<4, 4> Projection;

    void viewport(const int x, const int y, const int w, const int h) {
        Viewport = {{{w / 2.f, 0, 0, x + w / 2.f}, {0, h / 2.f, 0, y + h /
                                                                       2.f}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    }

    void projection(
            const float coeff) { // check https://github.com/ssloy/tinyrenderer/wiki/Lesson-4-Perspective-projection
        Projection = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, coeff, 1}}};
    }

    void lookat(const vec3f eye, const vec3f center,
                const vec3f up) { // check https://github.com/ssloy/tinyrenderer/wiki/Lesson-5-Moving-the-camera
        vec3f z = (eye - center).normalize();
        vec3f x = cross(up, z).normalize();
        vec3f y = cross(z, x).normalize();
        matf<4, 4> Minv = {{{x.x, x.y, x.z, 0}, {y.x, y.y, y.z, 0}, {z.x, z.y, z.z, 0}, {0, 0, 0, 1}}};
        matf<4, 4> Tr = {{{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}}};
        ModelView = Minv * Tr;
    }

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

    vec3f world2screen(vec3f v) {
        vec4f gl_vertex = embed<4>(v); // embed Vec3f to homogenius coordinates
        gl_vertex =
                Viewport * Projection * ModelView * gl_vertex; // transform it to screen coordinates
        vec3f v3 = proj<3>(gl_vertex / gl_vertex[3]); // transfromed vec3f vertex
        return vec3f(int(v3.x + .5), int(v3.y + .5), v3.z);
    }
}