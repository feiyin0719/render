//
// Created by yinfei on 2021/9/15.
//
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "camera.h"

#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H


class Render {
protected:
    TGAImage *image;

    Render();

    float *zbuffer;

    void init();

    std::vector<std::shared_ptr<Model>> models;

    vec3f *light_dir; // light source
    Camera camera;

    matf<4, 4> Viewport;
    matf<4, 4> Projection;

    void viewport(const int x, const int y, const int w, const int h);

    void projection(
            const float coeff);


public:
    Render(int width, int height);

    virtual void line(int x0, int y0, int x1, int y1, TGAColor &color);

    virtual void triangle(vec3f *u, TGAColor &color);

    virtual void triangle(vec3f *u, vec2f *texts, Model *model);

    virtual void addModel(std::string fileName);

    virtual void renderModels();

    virtual void renderModel(Model &model);

    virtual void lock();

    virtual void unlock();

    virtual void clear();

    void moveEye(float dx, float dy, float dz);

    virtual int getWidth() {
        return image->get_width();
    }

    virtual int getHeight() {
        return image->get_height();
    }

    virtual vec3f world2screen(vec3f v) {
        vec4f gl_vertex = embed<4>(v); // embed Vec3f to homogenius coordinates
        gl_vertex =
                Viewport * Projection * camera.getModelView() *
                gl_vertex; // transform it to screen coordinates
        vec3f v3 = proj<3>(gl_vertex / gl_vertex[3]); // transfromed vec3f vertex
        return vec3f(int(v3.x + .5), int(v3.y + .5), v3.z);
    }

    ~Render();

};


#endif //RENDER_RENDER_H
