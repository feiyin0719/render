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

void Render::triangle(vec3 *u, TGAColor &color) {
    GL::triangle(u, zbuffer, *(this->image), color);
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