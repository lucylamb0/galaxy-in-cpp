cmake_minimum_required(VERSION 3.16)
project(C_version)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(CMAKE_SYSTEM MATCHES Windows)
    message(STATUS "Target system is Windows")
endif()

if(CMAKE_HOST_SYSTEM MATCHES Linux)
    message(STATUS "Build host runs Linux")
endif()

message(STATUS ${CMAKE_HOST_SYSTEM})

find_package(Threads REQUIRED)
add_executable(C_version main.cpp Vector3.h Star.h includes.h Region.h)
if(TARGET Threads::Threads)
    target_link_libraries(C_version PRIVATE Threads::Threads)
endif()