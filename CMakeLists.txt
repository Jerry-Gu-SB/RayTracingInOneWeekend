cmake_minimum_required(VERSION 4.1)
project(RayTracingInOneWeekend)

set(CMAKE_CXX_STANDARD 20)

add_executable(RayTracingInOneWeekend main.cpp
        vec3.h
        color.h
        ray.h)
