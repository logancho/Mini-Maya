#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec4(0, 0, 1, 0);
    up = glm::vec4(0, 1, 0, 0);
    right = glm::vec4(1, 0, 0, 0);
    //
    xAxis = glm::vec4(1, 0, 0, 0);
    yAxis = glm::vec4(0, 1, 0, 0);
    zAxis = glm::vec4(0, 0, 1, 0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec4(0,0,10,1), glm::vec4(0,0,0,0), glm::vec4(0,1,0,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec4 &e, const glm::vec4 &r, const glm::vec4 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    theta(180),
    phi(0),
    xAxis(glm::vec4(1, 0, 0, 0)),
    yAxis(glm::vec4(0, 1, 0, 0)),
    zAxis(glm::vec4(0, 0, 1, 0)),
    eye(e),
    ref(r),
    world_up(worldUp),
    pan_amount(glm::vec4(0)),
    radius(glm::length(ref - eye))
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
//    look = glm::normalize(ref - eye);
//    right = glm::vec4(glm::normalize(glm::cross(glm::vec3(look), glm::vec3(world_up))), 0);
//    up = glm::vec4(glm::cross(glm::vec3(right), glm::vec3(look)), 0);
//    float tan_fovy = tan(glm::radians(fovy/2));
//    float len = glm::length(ref - eye);
//    aspect = width / static_cast<float>(height);
//    V = up*len*tan_fovy;
//    H = right*len*aspect*tan_fovy;

//    look = glm::normalize(ref - eye);
//    right = glm::vec4(glm::normalize(glm::cross(glm::vec3(look), glm::vec3(world_up))), 0);
//    up = glm::vec4(glm::cross(glm::vec3(right), glm::vec3(look)), 0);

    float tan_fovy = tan(glm::radians(fovy/2));

    aspect = width / static_cast<float>(height);
    V = up * radius * tan_fovy;
    H = right * radius * aspect * tan_fovy;

    eye =   glm::translate(glm::mat4(1.0f), glm::vec3(pan_amount)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(yAxis)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(xAxis)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(ref) - glm::vec3(0, 0, radius)) *
            glm::vec4(0, 0, 0, 1);

    look = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(yAxis)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(xAxis)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(ref) - glm::vec3(0, 0, radius)) *
            glm::vec4(0, 0, 1, 0);

    right = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(yAxis)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(xAxis)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(ref) - glm::vec3(0, 0, radius)) *
            glm::vec4(1, 0, 0, 0);

    up = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(yAxis)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(xAxis)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(ref) - glm::vec3(0, 0, radius)) *
            glm::vec4(0, 1, 0, 0);
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(glm::vec3(eye), glm::vec3(ref), glm::vec3(up));
}

void Camera::RotateAboutUp(float t)
{
//    glm::mat4 rotationTheta = glm::rotate(glm::mat4(1.0f), glm::radians(t), glm::vec3(up));
//    ref = ref - eye;
//    ref = rotationTheta * ref;
//    ref = ref + eye;
    theta += t;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float p)
{
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(right));
//    ref = ref - eye;
//    ref = rotation * ref;
//    ref = ref + eye;
    phi += p;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
//    glm::vec4 z = glm::vec4(glm::vec3(look) * amt, 1);
    radius -= amt;
//    eye += z;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec4 translation = glm::vec4(glm::vec3(right) * amt, 1);
//    eye += translation;
    pan_amount -= translation;
    ref -= translation;
}

void Camera::TranslateAlongUp(float amt)
{
    glm::vec4 translation = glm::vec4(glm::vec3(up) * amt, 1);
//    eye += translation;
    pan_amount -= translation;
    ref -= translation;
}
