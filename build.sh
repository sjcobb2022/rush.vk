#!/bin/bash
cmake -S . -B ./build
cmake --build . --target Shaders && cmake --build .
make && make Shaders && ./rush_vk