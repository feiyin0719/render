//
// Created by yinfei on 2021/9/15.
//

#include "render.h"
#include "gl.h"

Render::Render(int width, int height) {
    this->image = new TGAImage(width, height, 4);
    init();
}

Render::Render() {

}

void Render::init() {
    this->zbuffer = new float[getWidth() * getHeight()];
    for (int i = getHeight() * getWidth(); i--; zbuffer[i] = -std::numeric_limits<float>::max());
}

void Render::line(int x0, int y0, int x1, int y1, TGAColor &color) {
    GL::line(x0, y0, x1, y1, *(this->image), color);
}

void Render::triangle(vec3f *u, TGAColor &color) {
    GL::triangle(u, zbuffer, *(this->image), color);
}

void Render::triangle(vec3f *u, vec2f *texts, Model *model) {
    GL::triangle(u, zbuffer, *(this->image), texts, model);
}

void Render::addModel(std::string fileName) {
    Model *model = new Model(fileName);
    models.push_back(model);
}

void Render::renderModels() {
    for (auto iter = models.cbegin(); iter != models.cend(); iter++) {
        Model model = *(*iter);
        renderModel(model);
    }
}

void Render::renderModel(Model &model) {
    for (int i = 0; i < model.nfaces(); i++) {
        vec3f world_coords[3];
        vec3f screen_coords[3];
        vec2f texts[3];
        for (int j = 0; j < 3; j++) {
            world_coords[j] = model.vert(i, j);
            screen_coords[j] = GL::world2screen(world_coords[j]);
            texts[j] = model.uv(i, j);
        }
        triangle(screen_coords, texts, &model);

//        vec3f norm = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
//        norm.normalize();
//        float intensity = light * norm;
//        if (intensity > 0) {
//            TGAColor color(255 * intensity, 255 * intensity, 255 * intensity, 255);
//            ((Render *) render)->triangle(screen_coords, color);
//        }
    }
}

void Render::lock() {
    if (this->image != nullptr)
        this->image->lock();
}

void Render::unlock() {
    if (this->image != nullptr)
        this->image->unlock();
}

Render::~Render() {
    if (this->image != nullptr)
        delete this->image;
    if (this->zbuffer != nullptr)
        delete this->zbuffer;

}