//
// Created by jerry on 6/1/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_CAMERA_H
#define RAYTRACINGINONEWEEKEND_CAMERA_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "material.h"

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;  // Count of random samples for each pixel
    int max_depth = 10; // max number of ray bounces in a scene

    double vfov = 90; // vertical view angle or field of view

    point3 lookfrom = point3(0, 0, 0); // point where the camera is looking from
    point3 lookat = point3(0, 0, -1); // the point that we're looking at
    vec3 vup = vec3(0, 1, 0); // camera-relative "up" direction. note that this can be whatever we want

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int y = 0; y < image_height; y++) {
            if (y % 50 == 0) {
                std::clog << "\r\nScanlines remaining: " << (image_height - y) << '\n' << std::flush;
            }
            for (int x = 0; x < image_width; x++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(x, y);  // returns random ray in a unit square area around the original ray
                    pixel_color += ray_color(r, max_depth, world);  // adds that ray's color to the pixel color
                }
                // remember that pixel color is a "color" but it's really just a vec3 under the hood, and the pixel
                // colors don't get clamped until we call write_color.
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples. How much each sample is weighted in a weighted sum
    point3 camera_center; // camera center
    point3 pixel00_loc; // location of pixel 0,0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below

    vec3 u, v, w;       // These are the 3 vectors that determine our camera roll and angle
                        // u = camera right, v = camera up, w = camera opposite view direction (right hand rule), and camera center is at the origin

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;  // < 1 check

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = lookfrom;

        // determien viewport dimensions
        const auto focal_length = (lookfrom - lookat).length();  // distance between camera center, and the 2d plane of the viewport
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2.0);
        auto viewport_height = 2 * h * focal_length;
        const auto viewport_width = viewport_height * (double(image_width)/image_height);

        // u, v, w unit vectors
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate vectors across the horizontal and down the vertical viewport edges. The dimensions of our viewport as a vector
        const auto viewport_u = viewport_width * u; // vector going across the viewport edge of the rectangle
        const auto viewport_v = viewport_height * -v; // vector down viewport vertical edge of the rectangle. i think it's negative because we start top left and go down

        // Calculate the horizontal and vertical delta vectors from pixel to pixel. So how far apart each pixel is basically.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate location of upper left pixel. This is to help us with convention and to help us start from top left
        const auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

    }

    ray get_ray(int x, int y)  const {
        // Construct a camera ray from the origin directed at randomly sampled point around pixel location x, y

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                                + ((x + offset.x()) * pixel_delta_u)
                                + ((y + offset.y()) * pixel_delta_v);
        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5, -.5 ] - [+.5, +.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    // remember that the const at the end makes it a const function, meaning it's an error to write to its members
    color ray_color(const ray& r, const int depth, const hittable& world) {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            // So the record will come from whatever the ray has hit in the world. We'll then get the scattered ray
            // by asking what we've hit's material what the attenuation and ray scatter we got. Then we pipe that ray
            // into the ray color to get the direction, and the attenuation is the % multiplier how much falloff there
            // is for that ray's intensity. If for whatever reason the material isn't initialized, fallback to black.
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = .5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};


#endif // RAYTRACINGINONEWEEKEND_CAMERA_H
