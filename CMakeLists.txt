cmake_minimum_required(VERSION 3.27)
project(Graphics_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Graphics_Project main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
)
