//
// Created by jerry on 6/27/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class material {
public:
    virtual ~material() = default;

    // Attenuation is how much the wave gradually loses energy. So no attenuation means a ray is never scattered, and
    // absorbed instead.
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch zero scatter direction, which could result in very bad things later
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
private:
    // Albedo is fractional reflectance, a.k.a. how much light is reflected as like a mirror. So on a value from 0-1,
    // 1 means perfectly white reflecting all light, 0 means absorbs all light. Note that
    color albedo;
};
#endif // RAYTRACINGINONEWEEKEND_MATERIAL_H
