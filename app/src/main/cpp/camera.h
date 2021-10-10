//
// Created by yinfei on 2021/9/30.
//
#include "geometry.h"

#ifndef RENDER_CAMEARA_H
#define RENDER_CAMEARA_H


class Camera {
private:
    vec3f Position;
    vec3f TargetPositon;
    vec3f Front;
    vec3f Up;
    vec3f Right;
    vec3f WorldUp;
    // 欧拉角
    float Yaw;
    float Pitch;
    float Roll;

    void updateCameraVectors();


    matf<4, 4> ModelView;
    const float SPEED = 0.5f;
    const float DEGREE_SPEED = 1;
public:
    Camera();

    Camera(vec3f position, vec3f target, vec3f up, float pitch, float yaw, float roll);

    ~Camera();

    matf<4, 4> &getModelView();

    float getCoffee();

    void updateCamera(int dir);

    void updateYawPitch(float dx, float dy, bool constrainPitch = true);

};


#endif //RENDER_CAMEARA_H
