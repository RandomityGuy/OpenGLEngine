# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("OpenGLEngine")

function(advanced_option_off)
    foreach(arg IN LISTS ARGN)
        option(${arg} "" OFF)
        mark_as_advanced(FORCE ${arg})
    endforeach()
endfunction()

set(CMAKE_CXX_STANDARD 14)

advanced_option_off(GLFW_BUILD_DOCS GLFW_BUILD_EXAMPLES GLFW_BUILD_TESTS GLFW_USE_OSMESA GLFW_VULKAN_STATIC GLFW_INSTALL BUILD_SHARED_LIBS)

# Libraries
add_subdirectory(libs/glfw)
add_subdirectory(libs/tinyobjloader)

add_library(glad libs/glad/src/glad.c)

add_library(imgui 
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp

    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

# Include sub-projects.

set(ENGINE_SRC
    src/main.cpp

    src/window.cpp
    src/window.h
    src/shader.cpp
    src/shader.h
    src/vertexBuffer.h
    src/vertexBuffer.cpp
    src/vertexArray.cpp
    src/vertexArray.h
    src/indexBuffer.h
    src/indexBuffer.cpp
    src/GL.cpp
    src/GL.h
    src/texture2d.cpp
    src/texture2d.h
    src/camera.cpp
    src/camera.h
    src/mesh.cpp
    src/mesh.h
    src/model.cpp
    src/model.h
    src/object3d.cpp
    src/object3d.h
    src/scene.cpp
    src/scene.h
    src/light.cpp
    src/light.h
    src/frameBuffer.cpp
    src/frameBuffer.h
    src/renderBuffer.cpp
    src/renderBuffer.h
    src/pass.cpp
    src/pass.h
    src/renderContext.h
    src/renderer.cpp
    src/renderer.h
    src/defaultPass.cpp
    src/defaultPass.h
    src/postFXPass.cpp
    src/postFXPass.h
    src/cubeMap.cpp
    src/cubeMap.h
    src/skybox.cpp
    src/skybox.h
    src/refractMesh.cpp
    src/refractMesh.h
    src/refractModel.cpp
    src/refractModel.h
    src/refractPass.cpp
    src/refractPass.h
    src/gammaCorrectionPass.cpp
    src/gammaCorrectionPass.h
    src/passList.cpp
    src/passList.h
    
    src/stb_image.h
)

add_executable(engine ${ENGINE_SRC})
target_link_libraries(engine glfw glad imgui tinyobjloader)
target_include_directories(engine PRIVATE src)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${ENGINE_SRC})

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)
    set_property(TARGET engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/")
elseif (XCODE)
    set_target_properties(engine PROPERTIES 
        XCODE_GENERATE_SCHEME TRUE
        XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/"
    )
endif()

# OpenGL Library
if (APPLE)
    target_link_libraries(engine "-framework OpenGL")
elseif (WIN32)
    target_link_libraries(engine OpenGL32)
elseif (UNIX)
    target_link_libraries(engine GL)
endif()

add_custom_command(
        TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/shaders
                ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_command(
        TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/data
                ${CMAKE_CURRENT_BINARY_DIR}/data)

include_directories(
    libs/glfw/include
    libs/glm
    libs/glad/include
    libs/imgui
    libs/tinyobjloader
)

add_definitions(
    -DGLFW_STATIC 
    -DGLFW_INCLUDE_NONE
    -DIMGUI_IMPL_OPENGL_LOADER_GLAD
)