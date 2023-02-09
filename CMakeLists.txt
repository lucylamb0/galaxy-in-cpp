########### Flags ############################################################
set(SHOULD_USE_EXTERNALS ON)
set(SHOULD_USE_GTEST ON)
set(SHOULD_USE_MATPLOTLIB_CPP ON)
##############################################################################

cmake_minimum_required(VERSION 3.16)
project(GalaxySimulation)

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

if(SHOULD_USE_EXTERNALS)
    include(FetchContent)

    if(SHOULD_USE_GTEST)
        FetchContent_Declare(
                googletest
                # Specify the commit you depend on and update it regularly.
                URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    find_package(Matplot++ REQUIRED)


endif()


find_package(Threads REQUIRED)
add_executable(GalaxySimulation
        main.cpp
        includes/Vector3.h
        Star.h
        Star.cpp
        includes.h
        includes/Region.h
        includes/logging.h
        includes/RegionMatrix.h
        includes/json.h
        data_parser.cpp data_parser.h
        data_exporter.cpp data_exporter.h
        Simulator.cpp Simulator.h star_generator.h includes/Random.h
        Plotting.h source/RegionMatrix.cpp includes/ConfigHandler.cpp includes/ConfigHandler.h)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/external)

if(SHOULD_USE_EXTERNALS)
    if(SHOULD_USE_GTEST)
        enable_testing()
        add_executable(Simulation_Tests
                GTests/Vector3_Test.cc
                GTests/Star_Test.cc
                GTests/DataExporter_Test.cc
        )
        target_link_libraries(
                Simulation_Tests
                GTest::gtest_main
        )

        include(GoogleTest)
        gtest_discover_tests(Simulation_Tests)
    endif()

    if(SHOULD_USE_MATPLOTLIB_CPP)
#        find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)

#        target_include_directories(GalaxySimulation PRIVATE ${Python3_INCLUDE_DIRS} includes includes/external/)

        if(TARGET Threads::Threads)
            target_link_libraries(GalaxySimulation PUBLIC Threads::Threads)
        else()
            target_link_libraries(GalaxySimulation PUBLIC)
        endif()
    endif()
else()
    target_link_libraries(GalaxySimulation PUBLIC Threads::Threads)
endif()
