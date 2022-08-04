#cmake_minimum_required(VERSION 3.23)
cmake_minimum_required(VERSION 3.21)
project(C_version)

set(CMAKE_CXX_STANDARD 23)

add_executable(C_version main.cpp Vector3.h Star.h includes.h Region.h)
