cmake_minimum_required(VERSION 3.3)
# - Try to find OpenGL ES libraries
# Once done this will define
#  OGLES_FOUND - System has OpenGL ES
#  OGLES_INCLUDE_DIRS - The OpenGL ES include directories
#  OGLES_LIBRARIES - The libraries needed to use OpenGL ES
#  OGLES_DEFINITIONS - Compiler switches required for using OpenGL ES

set(OGLES_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/Include/")
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    if(CMAKE_GENERATOR MATCHES "Win64")
        set(OGLES_LIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/Windows/x86_64/Lib")
    else()
        set(OGLES_LIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/Windows/x86_32/Lib")
    endif()
endif()
find_library(EGL_LIB libEGL ${OGLES_LIBS_DIR} NO_DEFAULT_PATH)
find_library(OGLES2_LIB libGLESv2 ${OGLES_LIBS_DIR} NO_DEFAULT_PATH)
set(OGLES_LIBRARIES ${EGL_LIB} ${OGLES2_LIB})
set(OGLES_FOUND TRUE)