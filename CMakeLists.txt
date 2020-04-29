cmake_minimum_required(VERSION 3.16)
project(QHull)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)

add_subdirectory(pybind11)

pybind11_add_module(QHull binder.cpp QHull.h QHull.cpp utils.h utils.cpp)
target_link_libraries(QHull PUBLIC Eigen3::Eigen)
configure_file(${CMAKE_SOURCE_DIR}/QHull_py.py ${CMAKE_BINARY_DIR}/QHull_py.py)
configure_file(${CMAKE_SOURCE_DIR}/run.py ${CMAKE_BINARY_DIR}/run.py)
configure_file(${CMAKE_SOURCE_DIR}/utils.py ${CMAKE_BINARY_DIR}/utils.py)
