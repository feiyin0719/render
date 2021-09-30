//
// Created by yinfei on 2021/9/30.
//
#include "geometry.h"

#ifndef RENDER_CAMEARA_H
#define RENDER_CAMEARA_H


class Camera {
private:
    vec3f *eye; // camera position
    vec3f *center; // camera direction
    vec3f *up; // camera up vector
    matf<4, 4> ModelView;
public:
    Camera();

    Camera(vec3f eye, vec3f center, vec3f up);

    ~Camera();

    matf<4, 4> &getModelView();

    float getCoffee();

};


#endif //RENDER_CAMEARA_H
