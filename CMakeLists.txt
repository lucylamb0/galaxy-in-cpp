cmake_minimum_required(VERSION 3.16)
project(GalaxySimulation)

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
add_executable(GalaxySimulation
        main.cpp
        Vector3.h
        Star.h
        Star.cpp
        includes.h
        Region.h
        logging.h)

if(TARGET Threads::Threads)
    target_link_libraries(GalaxySimulation PRIVATE Threads::Threads)
endif()