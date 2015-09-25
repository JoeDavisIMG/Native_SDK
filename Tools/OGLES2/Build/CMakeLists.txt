cmake_minimum_required(VERSION 3.3)

########## Variables ##########
set(PVRTOOLS_BUILD_NAME "PVRTools")
project(${PVRTOOLS_BUILD_NAME})

include("${CMAKE_CURRENT_LIST_DIR}/../../../Builds/ogles.cmake")

########## Project build flags  ##########
add_definitions(-DBUILD_OGLES2)
set(RHI "OGLES2")

########## Include files ##########
set(PVRTOOLS_INCLUDE_DIRS "${OGLES_INCLUDE_DIRS}/" "${CMAKE_CURRENT_LIST_DIR}/../../" "${CMAKE_CURRENT_LIST_DIR}/../")
include_directories(${PVRTOOLS_INCLUDE_DIRS})

########## Source files ##########
set(SRC_FOLDER_NAME "src")
# Common
set(PVRTOOLS_SRC
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTBoneBatch.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTDecompress.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTError.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTFixedPoint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTGeometry.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTMatrixF.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTMatrixX.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTMisc.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTModelPOD.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTPFXParser.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTPrint3D.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTQuaternionF.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTQuaternionX.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTResourceFile.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTShadowVol.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTString.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTStringHash.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTTexture.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTTrans.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTTriStrip.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTUnicode.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTVector.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../PVRTVertex.cpp")
source_group("${SRC_FOLDER_NAME}" FILES ${PVRTOOLS_SRC})
# OpenGL ES 2.0
set(PVRTOOLS_RHI_SRC
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTBackground.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTPFXParserAPI.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTPFXSemantics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTPrint3DAPI.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTShader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTgles2Ext.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../PVRTTextureAPI.cpp")
source_group("${SRC_FOLDER_NAME}\\${RHI}" FILES ${PVRTOOLS_RHI_SRC})

add_library(${PVRTOOLS_BUILD_NAME} STATIC ${PVRTOOLS_SRC} ${PVRTOOLS_RHI_SRC})

# Link libraries
target_link_libraries(${PVRTOOLS_BUILD_NAME} ${OGLES_LIBRARIES})