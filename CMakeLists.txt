cmake_minimum_required(VERSION 2.8.12...3.22)
project(pcg VERSION 1.0.0 DESCRIPTION "Procedural generation" LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
target_compile_options(${target} PUBLIC 
$<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
$<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wconversion>
$<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wconversion>
$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wconversion>
)
endfunction()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)

set(HEADER_FILES
	include/Mesh.h
    include/Camera.h
    include/Shader.h
    include/FrameBuffer.h
    include/Texture.h
    include/Timer.h
    include/GBuffer.h
)

set(SOURCE_FILES
    main.cpp
    source/Mesh.cpp
    source/Camera.cpp
    source/Shader.cpp
    source/FrameBuffer.cpp
    source/Texture.cpp
    source/Timer.cpp
    source/GBuffer.cpp
)

# Add header with path to resources and add header to include directory
set(INCLUDE_RESOURCES_STRING ${CMAKE_CURRENT_LIST_DIR}/resources)
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/ResourcePath.h.in
    ${CMAKE_BINARY_DIR}/generated/ResourcePath.h @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

add_executable(pcg ${SOURCE_FILES} ${HEADER_FILES})

enable_warnings(pcg)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pcg)

target_include_directories(pcg PRIVATE include ${GLM_INCLUDE_DIR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(pcg PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(pcg PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(pcg PRIVATE imgui::imgui)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(pcg PRIVATE assimp::assimp)
