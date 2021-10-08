//
// Created by yinfei on 2021/9/15.
//

#include "render.h"
#include "gl.h"

void Render::viewport(const int x, const int y, const int w, const int h) {
    Viewport = {{{w / 2.f, 0, 0, x + w / 2.f}, {0, h / 2.f, 0, y + h /
                                                                   2.f}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

void Render::projection(
        const float coeff) { // check https://github.com/ssloy/tinyrenderer/wiki/Lesson-4-Perspective-projection
    Projection = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, coeff, 1}}};
}


void Render::updateCamera(int dir) {
    camera.updateCamera(dir);
    projection(camera.getCoffee());
}

void Render::updateYawPitch(float dx, float dy, bool constrainPitch) {
    camera.updateYawPitch(dx, dx, constrainPitch);
}


Render::Render(int width, int height) {
    this->image = new TGAImage(width, height, 4);
    init();
}

Render::Render() {

}

void Render::init() {
    this->zbuffer = new float[getWidth() * getHeight()];
    for (int i = getHeight() * getWidth(); i--; zbuffer[i] = -std::numeric_limits<float>::max());
    light_dir = new vec3f(1, 1, 1);
    viewport(getWidth() / 8, getHeight() / 8, getWidth() * 3 / 4,
             getHeight() * 3 / 4);
    projection(camera.getCoffee());
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
    std::shared_ptr<Model> ptr = std::make_shared<Model>(Model(fileName));
    models.push_back(ptr);
}

void Render::renderModels() {
    for (int i = getHeight() * getWidth(); i--; zbuffer[i] = -std::numeric_limits<float>::max());
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
            screen_coords[j] = world2screen(world_coords[j]);
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

void Render::clear() {
    this->image->clear();
}

Render::~Render() {
    if (this->image != nullptr)
        delete this->image;
    if (this->zbuffer != nullptr)
        delete this->zbuffer;
    if (this->light_dir != nullptr)
        delete this->light_dir;

}