########### Flags ############################################################
set(SHOULD_USE_GTEST ON)
##############################################################################

cmake_minimum_required(VERSION 3.16)
project(GalaxySimulation)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

######### Set Compiler Options #########
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

############# Detect OS ###############
message(STATUS ${CMAKE_HOST_SYSTEM})
if(CMAKE_SYSTEM MATCHES Windows)
    message(STATUS "Target system is Windows")
elseif(CMAKE_SYSTEM MATCHES Linux)
    message(STATUS "Target system is Linux")
elseif(CMAKE_SYSTEM MATCHES Darwin)
    message(STATUS "Target system is MacOS")
else()
    message(FATAL_ERROR "Target system is not supported")
endif()
######################################

find_package(Threads REQUIRED)
add_executable(
        GalaxySimulation

        main.cpp
        Star.cpp Star.h
        includes.h
        includes/Vector3.h
        includes/Region.h
        includes/logging.h
        includes/RegionMatrix.h source/RegionMatrix.cpp
        includes/json.h
        data_parser.cpp data_parser.h
        data_exporter.cpp data_exporter.h
        Simulator.cpp Simulator.h
        star_generator.h
        includes/Random.h

        includes/ConfigHandler.h

        includes/ConfigStruct.h source/star_generator.cpp)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/external)

target_link_libraries(GalaxySimulation PUBLIC Threads::Threads)

include(./GTests/CMakeLists.txt)
