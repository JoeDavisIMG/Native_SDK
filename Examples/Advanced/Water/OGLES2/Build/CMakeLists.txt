cmake_minimum_required(VERSION 3.3)

########## Project variable setup  ##########
set(DEMO_BUILD_NAME "Water")
project(${DEMO_BUILD_NAME})
set(TARGET_OS "Windows")
set(PVR_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../")
set(PVRVFRAME_DIR "")

########## Includes ##########
include("${PVR_SDK_DIR}/Builds/ogles.cmake")
include_directories(${OGLES_INCLUDE_DIRS})
set(PVRTOOLS_INCLUDE_DIRS "${PVR_SDK_DIR}/Tools/" "${PVR_SDK_DIR}/Tools/OGLES2/")
include_directories(${PVRTOOLS_INCLUDE_DIRS})
include_directories("${PVR_SDK_DIR}/Shell/")
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    include_directories("${PVR_SDK_DIR}/Shell/OS/Windows/")
    include_directories("${PVR_SDK_DIR}/Shell/API/KEGL/")
endif()

########## Project build flags  ##########
add_definitions(-DBUILD_OGLES2)

########## PVRTools and PVRShell ##########
file(GLOB PVR_SHELL
    "${PVR_SDK_DIR}/Shell/*.h"
    "${PVR_SDK_DIR}/Shell/*.cpp")
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    file(GLOB PVR_SHELL_OS
    "${PVR_SDK_DIR}/Shell/OS/Windows/*"
    "${PVR_SDK_DIR}/Shell/API/KEGL/*")
endif()
set(PVR_SHELL ${PVR_SHELL} ${PVR_SHELL_OS})
source_group(pvr\\shell FILES ${PVR_SHELL})
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    file(GLOB PVR_RESOURCES
    "${PVR_SDK_DIR}/Builds/Windows/Resources/*")
endif()
source_group(pvr\\resources FILES ${PVR_RESOURCES})

########## Source files ##########
set(DEMO_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(DEMO_INC
    )
source_group(demo\\inc FILES ${DEMO_INC})
set(DEMO_SRC
    ${DEMO_SRC_ROOT}/OGLES2Water.cpp)
source_group(demo\\src FILES ${DEMO_SRC})

########## Resources ##########
set(COOKED_ASSET_DIR "${DEMO_SRC_ROOT}")
file(GLOB COOKED_ASSETS
    "${COOKED_ASSET_DIR}/*.fsh"
    "${COOKED_ASSET_DIR}/*.vsh"
	"${COOKED_ASSET_DIR}/*.pfx"
	"${COOKED_ASSET_DIR}/*.pod"
	"${COOKED_ASSET_DIR}/*.pvr")

########## Add source to exe ##########
set(DEMO_EXE_PROPERTY "")
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(DEMO_EXE_PROPERTY WIN32)
endif()
set(DEMO_INCLUDE_FILES ${DEMO_INC})
set(DEMO_SOURCE_FILES ${DEMO_SRC} ${PVR_SHELL})
set(DEMO_RESOURCE_FILES ${COOKED_ASSETS} ${PVR_RESOURCES})
add_subdirectory("${PVR_SDK_DIR}/Tools/OGLES2/" "PVRTools")
add_executable(${DEMO_BUILD_NAME} ${DEMO_EXE_PROPERTY} ${DEMO_INCLUDE_FILES} ${DEMO_SOURCE_FILES})
target_link_libraries(${DEMO_BUILD_NAME} ${OGLES_LIBRARIES})
target_link_libraries(${DEMO_BUILD_NAME} "PVRTools")

########## Install  ##########
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Debug")
endif()
# Copy PVRVFrame DLLs to target directory
if(PVRVFRAME_DIR STREQUAL "")
    message("WARNING: OpenGL ES emulation libraries will not be installed. Please add them to your path to run the demo")
else() 
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        if(CMAKE_GENERATOR MATCHES "Win64")
            file(GLOB PVRVFRAME_LIBS "${PVRVFRAME_DIR}/Library/Windows_x86_64/*.dll")
        else()
            file(GLOB PVRVFRAME_LIBS "${PVRVFRAME_DIR}/Library/Windows_x86_32/*.dll")
        endif()
    endif()
    install(FILES ${PVRVFRAME_LIBS} DESTINATION "${OUTPUT_DIR}")
endif()
install(FILES ${DEMO_RESOURCE_FILES} DESTINATION "${OUTPUT_DIR}")
