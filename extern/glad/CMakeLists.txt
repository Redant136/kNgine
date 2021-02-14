cmake_minimum_required(VERSION 3.0)
project(Glad)

add_library(kNgine_glad include/glad/glad.h src/glad.c)
target_include_directories(kNgine_glad PUBLIC include/)
