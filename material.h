//
// Created by jerry on 6/27/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "color.h"
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

        scattered = ray(rec.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
private:
    // Albedo is fractional reflectance, a.k.a. how much light is reflected as like a mirror. So on a value from 0-1,
    // 1 means perfectly white reflecting all light, 0 means absorbs all light. Note that
    color albedo;
};

class metal : public material {
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (random_unit_vector() * fuzz);
        scattered = ray(rec.point, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)

    const override {
        attenuation = color(1.0, 1.0, 1.0);

        // if we're inside the material or outside the material. If we're inside the material, the ratio flips as
        // Snell's goes like start_eta/end_eta, so we need to flip inverse depending on where we're coming from
        double this_refraction_index = rec.outward_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        vec3 refracted = refract(unit_direction, rec.normal, this_refraction_index);

        scattered = ray(rec.point, refracted);

        return true;
    }

private:
    // It's either the refractive index of the material in a vacuum, or the ratio of the material's index over the
    // surrounding media's refractive index
    double refraction_index;
};


#endif // RAYTRACINGINONEWEEKEND_MATERIAL_H
