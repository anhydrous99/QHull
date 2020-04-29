cmake_minimum_required(VERSION 3.16)
project(QHull)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)

add_subdirectory(pybind11)

pybind11_add_module(QHull main.cpp QHull.h QHull.cpp)
target_link_libraries(QHull PUBLIC Eigen3::Eigen)
