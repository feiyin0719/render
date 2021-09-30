#include "geometry.h"

template<typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2) {
    return vec<3, T>{v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x};
}

matf<4, 4> lookAt(vec3f &eye, vec3f &center, vec3f &up) {
    vec3f z = (eye - center).normalize();
    vec3f x = cross(up, z).normalize();
    vec3f y = cross(z, x).normalize();
    matf<4, 4> Minv = {{{x.x, x.y, x.z, 0},
                               {y.x, y.y, y.z, 0},
                               {z.x, z.y, z.z, 0},
                               {0, 0, 0, 1}}};
    matf<4, 4> Tr = {{{1, 0, 0, -center.x},
                             {0, 1, 0, -center.y},
                             {0, 0, 1, -center.z},
                             {0, 0, 0, 1}}};
    return Minv * Tr;
}

