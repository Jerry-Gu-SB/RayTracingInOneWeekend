//
// Created by jerry on 5/14/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_SPHERE_H
#define RAYTRACINGINONEWEEKEND_SPHERE_H

#include "hittable.h"

class sphere: public hittable {
    public:
        // so this syntax is just initializing the private variables immediately upon creation
        // you can see that the {} at the end just signify and empty constructor, since we've done all we need to do
        // already in the initialization
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)){}

        bool hit(const ray& ray, double ray_tmin, double ray_tmax, hit_record& hit_record) const override {
            vec3 C_MINUS_Q = center - ray.origin();
            auto a = ray.direction().length_squared();
            auto h = dot(ray.direction(), C_MINUS_Q);
            auto c = C_MINUS_Q.length_squared() - radius * radius;

            auto discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }

            auto sqrt_discriminant = std::sqrt(discriminant);


            // this defintiion and the if statement below is just checking both roots and seeing if either fall within
            // the given t range
            auto root = (h - sqrt_discriminant) / a;

            if (root <= ray_tmin || ray_tmax <= root) {
                root = (h + sqrt_discriminant) / a; // so this is just checking the positive root
                if (root <= ray_tmin || ray_tmax <= root) {
                    return false;
                }
            }

            hit_record.t = root;
            hit_record.point = ray.at(hit_record.t);
            // magnitude from root to center will always be radius, so divide to get unit vector
            vec3 outward_normal = (hit_record.point - center) / radius;
            hit_record.set_face_normal(ray, outward_normal);

            return true;
        }
    private:
        point3 center;
        double radius;
};

#endif // RAYTRACINGINONEWEEKEND_SPHERE_H
