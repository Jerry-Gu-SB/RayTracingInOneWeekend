//
// Created by jerry on 4/5/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H
#include "vec3.h"
#include "interval.h"

using color = vec3;

// So our image viewer is kind of dark because we're feeding it linear images, but it's then displaying those images in
// a gamma space. So we're correcting it by transforming it to gamma n, where the formula is (1/gamma)^n. Here it's
// gamma 2 = (1 / gamma)^2 = sqrt(gamma). Images do this by default because our eyes don't perceive light linearly.
// Pretty nuts stuff.
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);
    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Transforms colors from linear --> gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif // RAYTRACINGINONEWEEKEND_COLOR_H
