#!/bin/bash
FILE="image.ppm"

if [ -f "$FILE" ]; then
    rm "$FILE"
    echo "$FILE has been deleted."
else
    echo "$FILE does not exist."
fi

cmake --build build

./build/RayTracingInOneWeekend >> image.ppm