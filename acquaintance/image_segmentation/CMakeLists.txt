cmake_minimum_required(VERSION 3.16.0)

project(image_segmentation C)

add_executable(image_seg main.c)
target_link_libraries(image_seg m)
target_compile_options(image_seg PRIVATE -Wall -Wno-unknown-pragmas)