cmake_minimum_required(VERSION 3.21)
project(C_version)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM MATCHES Windows)
    message(STATUS "Target system is Windows")
endif()

if(CMAKE_HOST_SYSTEM MATCHES Linux)
    message(STATUS "Build host runs Linux")
endif()

message(STATUS ${CMAKE_HOST_SYSTEM})

add_executable(C_version main.cpp Vector3.h Star.h includes.h Region.h)
