//
// Created by yinfei on 2021/9/30.
//

#include "camera.h"

Camera::Camera(vec3f position, vec3f target, vec3f up, float pitch, float yaw, float roll) {
    this->Position = position;
    this->WorldUp = up;
    this->TargetPositon = target;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->Roll = roll;
    updateCameraVectors();
}

Camera::Camera() :
        Camera(vec3f(0, 1, 2),
               vec3f(0, 0, 0),
               vec3f(0, 1, 0),
               0, -100, 0) {


}

Camera::~Camera() {

}

matf<4, 4> &Camera::getModelView() {
    return ModelView;
}

float Camera::getCoffee() {
    return -1.f / (Position - TargetPositon).norm();
}

void Camera::updateCamera(int dir) {
    if (dir == 0) {
        this->Position = Position + Front * SPEED;
    } else if (dir == 1) {
        this->Position = Position - Front * SPEED;
    } else if (dir == 2) {
        Position = Position + Right * SPEED;
    } else if (dir == 3) {
        Position = Position - Right * SPEED;
    }

    updateCameraVectors();
}

void Camera::updateYawPitch(float dx, float dy, bool constrainPitch) {
    Yaw += dx * DEGREE_SPEED;
    Pitch += dy * DEGREE_SPEED;
    if (constrainPitch) {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
// 计算新的视角矩阵
    vec3f front;
    front.x = cos(radians(this->Yaw)) * cos(radians(this->Pitch));
    front.y = sin(radians(this->Pitch));
    front.z = sin(radians(this->Yaw)) * cos(radians(this->Pitch));
    this->Front = front.normalize();
//this->TargetPositon = this->TargetPositon - this->Front;
/*this->WorldUp.x = sin(glm::radians(Roll));
this->WorldUp.y = cos(glm::radians(Roll));
this->WorldUp.z = 0;*/
    this->Right = cross(this->Front, this->WorldUp).normalize();  // 向量归一化
    this->Up = cross(this->Right, this->Front).normalize();
    vec3f c = Position + Front;
    ModelView = lookAt(Position, c, Up);
}