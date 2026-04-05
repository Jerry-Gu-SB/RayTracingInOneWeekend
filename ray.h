//
// Created by jerry on 4/5/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_RAY_H
#define RAYTRACINGINONEWEEKEND_RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(double t) const {
        return orig + t*dir;
    }

private:
    point3 orig;
    vec3 dir;
};

#endif // RAYTRACINGINONEWEEKEND_RAY_H
