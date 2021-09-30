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

void
Render::lookat() { // check https://github.com/ssloy/tinyrenderer/wiki/Lesson-5-Moving-the-camera
    vec3f z = (*eye - *center).normalize();
    vec3f x = cross(*up, z).normalize();
    vec3f y = cross(z, x).normalize();
    matf<4, 4> Minv = {{{x.x, x.y, x.z, 0}, {y.x, y.y, y.z, 0}, {z.x, z.y, z.z, 0}, {0, 0, 0, 1}}};
    matf<4, 4> Tr = {{{1, 0, 0, -center->x}, {0, 1, 0, -center->y}, {0, 0, 1, -center->z}, {0, 0, 0, 1}}};
    ModelView = Minv * Tr;
}

void Render::initViewPort() {
    lookat();                            // build the ModelView matrix
    viewport(getWidth() / 8, getHeight() / 8, getWidth() * 3 / 4,
             getHeight() * 3 / 4); // build the Viewport matrix
    projection(-1.f / (*eye - *center).norm());
}

void Render::moveEye(float dx, float dy, float dz) {
    eye->x += dx;
    eye->y += dy;
    eye->z += dz;
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
    eye = new vec3f(2, 1, 3);
    center = new vec3f(0, 0, 0);
    up = new vec3f(0, 1, 0);
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
    if (this->eye != nullptr)
        delete this->eye;
    if (this->center != nullptr)
        delete this->center;
    if (this->up != nullptr)
        delete this->up;
}