// Implementation of https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"


color ray_color(const ray& ray, const hittable& world) {
    hit_record rec;

    if (world.hit(ray, 0, infinity, rec)) {
        return .5 * (rec.normal + color(1, 1, 1));
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

    // World
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}