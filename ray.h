//
// Created by Ryan M. on 9/21/25.
//

#ifndef GRAPHICS_PROJECT_RAY_H
#define GRAPHICS_PROJECT_RAY_H

#include "vec3.h"


class ray {
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction, double time)
            : orig(origin), dir(direction), tm(time) {}

        ray(const point3& origin, const vec3& direction)
            : ray(origin, direction, 0) {}

        const point3& origin() const {return orig; }
        const vec3& direction() const {return dir;}

        double time() const { return tm; }

        point3 at (double t) const {
            return orig + t * dir;
        }

    private:
        point3 orig;
        vec3 dir;
        double tm;

};

#endif //UNTITLED1_RAY_H
