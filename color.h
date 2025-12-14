//
// Created by Ryan M. on 9/14/25.
//

#ifndef GRAPHICS_PROJECT_COLOR_H
#define GRAPHICS_PROJECT_COLOR_H

#include "vec3.h"
#include "interval.h"
#include "rtweekend.h"


using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if(linear_component > 0){
        return std::sqrt(linear_component);
    }
    return 0.0;
}

inline color hexConvert(int hexValue) {
    double r = ((hexValue >> 16) & 0xFF) / 255.0;
    double g = ((hexValue >> 8) & 0xFF) / 255.0;
    double b = (hexValue & 0xFF) / 255.0;
    return color(r, g, b);
}

void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Clamp to [0.0,0.999] and convert to byte [0,255]
    static const interval intesety(0.0, 0.999);
    int rbyte = int(255.99 * intesety.clamp(r));
    int gbyte = int(255.99 * intesety.clamp(g));
    int bbyte = int(255.99 * intesety.clamp(b));
    
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif //GRAPHICS_PROJECT_COLOR_H
