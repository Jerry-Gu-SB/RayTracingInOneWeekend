//
// Created by jerry on 5/14/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

class hit_record {
    public:
        point3 point;
        vec3 normal;
        double t;
        bool outward_face;

        // Sets hit record normal vector, always out
        // NOTE: outward_normal is assumed to have unit length
        // we could add a function to normalize outward_normal
        // but it's more convenient and efficient if the geometry code
        // does this instead
        void set_face_normal(const ray& ray, const vec3& outward_normal) {

            // If the ray is inside surface, then outward normal is pointing out in the
            // same sort of direction, so dot is positive. Otherwise the outward normal
            // is always against the ray coming from the outside, therefore dot is negative.
            outward_face = dot(ray.direction(), outward_normal) < 0;
            normal = outward_face ? outward_normal : -outward_normal;
        }
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
