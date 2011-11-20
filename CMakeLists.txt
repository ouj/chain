cmake_minimum_required(VERSION 2.8)

# main project name defined
project(angryman)

# osx architectures
if(NOT BUILD_NEWGCC)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()

# output directories
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    #set(BIN_DIR_SUFFIX mk)
    set(MAKEFILES ON)
    set(CMAKE_VERBOSE_MAKEFILE ${BUILD_VERBOSE})
elseif(CMAKE_GENERATOR STREQUAL "Xcode")
    #set(BIN_DIR_SUFFIX xcode)
    set(XCODE ON)
elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${BIN_DIR_SUFFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${BIN_DIR_SUFFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${BIN_DIR_SUFFIX})

#include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/kinect/include)
include_directories(${PROJECT_SOURCE_DIR}/kinect/include/openni)
include_directories(/homebrew/include)
include_directories(/usr/local/include)

#library directories
link_directories(${PROJECT_SOURCE_DIR}/kinect/lib)
link_directories(/homebrew/lib)
link_directories(/usr/local/lib)

# libraries
find_package(GLUT)
find_package(OpenGL)

GET_FILENAME_COMPONENT(KINECT_DEP ${PROJECT_SOURCE_DIR}/kinect ABSOLUTE)

if(WIN32)
    GET_FILENAME_COMPONENT(GL_PATH ../OpenGL ABSOLUTE)
    include(${GL_PATH}/use_gl.cmake)
endif()

# compiler flags
if(MAKEFILES)
    if(NOT BUILD_NEWGCC)
        add_definitions(-O3)
        add_definitions(-mfpmath=sse)
        add_definitions(-funroll-loops)
        add_definitions(-ftree-vectorize)
    
        add_definitions(-funsafe-loop-optimizations)
        add_definitions(-fstrict-aliasing)
        add_definitions(-ffast-math)
    else()
        add_definitions(-Ofast)
    endif()
endif()

if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

# set default build to release
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)
