//
// Created by yinfei on 2021/9/30.
//

#include "camera.h"

Camera::Camera(vec3f eye, vec3f center, vec3f up) {
    this->eye = new vec3f(eye.x, eye.y, eye.z);
    this->center = new vec3f(center.x, center.y, center.z);
    this->up = new vec3f(up.x, up.y, up.z);
    ModelView = lookAt(eye, center, up);
}

Camera::Camera() :
        Camera(vec3f(2, 1, 3),
               vec3f(0, 0, 0),
               vec3f(0, 1, 0)) {


}

Camera::~Camera() {
    if (this->eye != nullptr)
        delete this->eye;
    if (this->center != nullptr)
        delete this->center;
    if (this->up != nullptr)
        delete this->up;
}

matf<4, 4> &Camera::getModelView() {
    return ModelView;
}

float Camera::getCoffee() {
    return -1.f / (*eye - *up).norm();
}