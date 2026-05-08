// Implementation of https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

double hit_sphere(const point3& center, const double radius, const ray& ray) {
    const vec3 C_minus_Q = center - ray.origin();  // center, as in center of the SPHERE, C

    // abc are all derived here from the quadratic dot product expansion of the definition of a sphere defined using
    // our vectors. The variables have little semantic value alone, but as a whole the discriminant lets us know
    // whether we're inside or outside a sphere
    const auto a = dot(ray.direction(), ray.direction());
    const auto b = -2.0 * dot(ray.direction(), C_minus_Q);
    const auto c = dot(C_minus_Q, C_minus_Q) - radius * radius;
    const auto discriminant = b * b - 4 * a * c;

    // if discriminant < 0, no solutions, == 0 on the sphere, > 0, inside the sphere
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0 * a);  // returns the solution
    }
}


color ray_color(const ray& ray) {
    // remember t is the point on the ray  P(t) = A + tb
    auto t = hit_sphere(point3(0, 0, -1), 0.5, ray);

    // so the ray here, is from the camera to the plane, and if imagine there's a sphere existing as we've defined
    // above, if we've hit the edge of the sphere, then t == 0, if it's greater than we're inside as t is now hitting
    // 2 points inside the sphere, and if it's less than zero, then we're outside the sphere.
    if (t > 0.0) {
        vec3 N = unit_vector(ray.at(t) - vec3(0, 0, -1));

        // +1: why +1 and * 0.5? Remember that unit vectors can go from [-1, 1] in each component.
        // So if we have -1 in the x component, we get +1 and zeroed out to no Red
        // if we have 1 in the x component, we get 2, and then get normalized down by the .5, as write_color() only
        // works on values between [0, 1] as it normalizes them to [0, 255]. see color.h. Note that color is just an
        // alias for vec3 as well.
        return 0.5 * color ( N.x() + 1, N.y() + 1, N.z() + 1);
    }
    const vec3 unit_direction = unit_vector(ray.direction());
    const auto a = .5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    // Image

    const auto aspect_ratio = 16.0 / 9.0;

    const int image_width = 400;

    // Calculate image height, ensure it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;  // < 1 check


    // Camera
    const auto focal_length = 1.0;  // distance between camera center, and the 2d plane of the viewport
    const auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * (double(image_width)/image_height);
    const auto camera_center = point3(0, 0, 0);  // a.k.a. the "eye point"


    // Calculate vectors across the horizontal and down the vertical viewport edges. The dimensions of our viewport as a vector
    const auto viewport_u = vec3(viewport_width, 0, 0);
    const auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel. So how far apart each pixel is basically.
    const auto pixel_delta_u = viewport_u / image_width;
    const auto pixel_delta_v = viewport_v / image_height;

    // Calculate location of upper left pixel. This is to help us with convention and to help us start from top left
    const auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    const auto pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = 0; y < image_height; y++) {
        if (y % 50 == 0) {
            std::clog << "\r\nScanlines remaining: " << (image_height - y) << '\n' << std::flush;
        }
        for (int x = 0; x < image_width; x++) {
            auto pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);  // the delta u/v is the distance between pixels
            auto ray_direction = pixel_center - camera_center;

            // So here at this point, imagine we have a camera that's shooting laters from our eyes over each pixel.
            // that's what this ray is doing.
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}