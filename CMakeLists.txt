# File: CMakeLists.txt
# Project: GEA_Compiler
# Created Date: 10/28/2023
# Author: Luke Houston
# -----
# Last Modified: 10/31/2023
# Modified By: Luke Houston
# -----
# License: MIT License
# -----
# Description: CMakeLists.txt for GEA_Compiler

# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(gea_compiler VERSION 0.1.0)
add_compile_definitions(GEA_COMPILER_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_compile_definitions(GEA_COMPILER_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_compile_definitions(GEA_COMPILER_VERSION_PATCH=${PROJECT_VERSION_PATCH})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set the source directory
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Set the include directory
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Set the test directory
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)

# Set the library directory
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

# Set compiler flags for debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)

    if(MSVC)
        # Compiler is Visual Studio
        add_compile_options(/Zi /Od)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Compiler is gcc/g++
        add_compile_options(-g -O0 -Wall -Wno-unused)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Compiler is Clang
        add_compile_options(-g -O0 -Weverything)
    endif()
endif()

# Set compiler flags for release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)

    if(MSVC)
        # Compiler is Visual Studio
        add_compile_options(/O2 /DNDEBUG)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Compiler is gcc/g++
        add_compile_options(-O3 -DNDEBUG -mwindows)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Compiler is Clang
        add_compile_options(-O3 -DNDEBUG -mwindows)
    endif()
endif()

# Get all source files
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h ${INCLUDE_DIR}/*.h)

# Add executable
add_executable(gea_compiler ${SOURCES})

# Add library
list(REMOVE_ITEM SOURCES ${SOURCE_DIR}/main.cpp)
add_library(gea_compiler_lib STATIC ${SOURCES})

# Add include directories
target_include_directories(gea_compiler PUBLIC ${INCLUDE_DIR})
target_include_directories(gea_compiler_lib PUBLIC ${INCLUDE_DIR})

# Add external libraries
add_subdirectory(${LIB_DIR}/glm)
add_subdirectory(${LIB_DIR}/lz4/build/cmake)
add_subdirectory(${LIB_DIR}/rapidjson)
target_include_directories(gea_compiler PUBLIC ${LIB_DIR}/stb)
add_subdirectory(${LIB_DIR}/tinyobjloader)

# Link libraries
target_link_libraries(gea_compiler PUBLIC
    gea_compiler_lib
    glm
    lz4
    RapidJSON
    tinyobjloader
)

# Uninstall target
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(gea_uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake)

# Add tests
add_subdirectory(${TEST_DIR})

# Set the working directory to the project directory
set_target_properties(gea_compiler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Enable testing
enable_testing()