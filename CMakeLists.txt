cmake_minimum_required(VERSION 3.23)
project(C_version)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies
add_subdirectory (external)

if(INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
        imgui/
        external/AntTweakBar-1.16/include/
        external/glfw-master/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

# Tutorial 6
add_executable(tutorial06_keyboard_and_mouse
        tutorial06_keyboard_and_mouse/tutorial06.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.hpp
        common/objloader.cpp

        TransformVertexShader.vertexshader
        tutorial06_keyboard_and_mouse/TextureFragmentShader.fragmentshader
        common/object_loader.cpp common/object_loader.h

        OLD/Model.cpp
        OLD/Model.h

        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3_loader.h
        imgui/imgui_internal.h

        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        )
target_link_libraries(tutorial06_keyboard_and_mouse
        ${ALL_LIBS}
        )

set(CMAKE_CXX_STANDARD 23)

link_directories(C_version ${PROJECT_SOURCE_DIR}/lib)

add_executable(C_version
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.hpp
        common/objloader.cpp

        TransformVertexShader.vertexshader
        tutorial06_keyboard_and_mouse/TextureFragmentShader.fragmentshader
        common/object_loader.cpp common/object_loader.h

        Model.cpp
        Model.h

        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3_loader.h
        imgui/imgui_internal.h

        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h

        main.cpp Vector3.h Star.h includes.h Region.h Utils.h

        MilkshapeModel.h
        MilkshapeModel.cpp

        config.cpp config.h globals.h)
target_link_libraries(C_version
        ${ALL_LIBS}
        )

