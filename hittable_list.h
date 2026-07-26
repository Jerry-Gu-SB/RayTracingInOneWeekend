//
// Created by jerry on 5/15/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H

#include "hittable.h"
#include <vector>

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() = default;
        hittable_list(shared_ptr<hittable> object) { add(object);}

        void clear() { objects.clear(); }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit(const ray& ray, interval ray_t, hit_record& record) const override {
            hit_record temp_record;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object: objects) {
                if (object->hit(ray, interval(ray_t.min, closest_so_far), temp_record)) {
                    hit_anything = true;
                    closest_so_far = temp_record.t;
                    record = temp_record;
                }
            }
            return hit_anything;
        }

};

#endif // RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
