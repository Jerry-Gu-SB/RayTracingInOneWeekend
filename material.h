//
// Created by jerry on 6/27/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    // attenuation is how much the wave gradually loses energy
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};
#endif // RAYTRACINGINONEWEEKEND_MATERIAL_H
