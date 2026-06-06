//
// Created by jerry on 5/22/2026.
//

#ifndef RAYTRACINGINONEWEEKEND_INTERVAL_H
#define RAYTRACINGINONEWEEKEND_INTERVAL_H

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // default empty interval

    interval(double min, double max) : min(min), max(max) {}

    // the const on this line means that any variables it references are reference as read-only
    // You should do this as much as possible to keep things from being modified accidentally
    double size() const {
        return max - min;
    }

    bool contains(const double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(const double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};
// so you can't initialize these within the class because when it makes the class, it doesn't know how much memory to
// allocate. Therefore, you make the incomplete type above, then calls the constructors after.
const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif // RAYTRACINGINONEWEEKEND_INTERVAL_H
