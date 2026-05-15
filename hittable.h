//
// Created by jerry on 5/14/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 point;
        vec3 normal;
        double t;
};

class hittable {
    public:
        // ~foo() is a destructor class. Essentially it defines how we should cleanup this class
        // this should only be done pretty much if you define a virtual function like we do below, as destroying a
        // a class with a virtual function through a non-virtual destructor is undefined in CPP.
        // https://stackoverflow.com/questions/22491174/when-do-we-need-to-define-destructors

        // remember virtual functions are defined here but will be implemented by its derived subclass.
        // ex. virtual eat() function implemented by a wolf() or a sheep() subclass.

        virtual ~hittable() = default;

        virtual bool hit(const ray& ray, double ray_tmin, double ray_tmax, hit_record& hit_record) const = 0;
};

#endif // RAYTRACINGINONEWEEKEND_HITTABLE_H
