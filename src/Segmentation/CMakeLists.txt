
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# Library files
set(HEADERS
  XVColorSeg.h
  XVMotionSeg.h
  XVScalarSeg.h
  XVSegmentation.h
  )

set(SOURCES
  XVSegmentation.cc
  XVColorSeg.cc
  XVMotionSeg.cc
  XVScalarSeg.cc
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/Tools
  ${PROJECT_SOURCE_DIR}/src/Images
  ${PROJECT_SOURCE_DIR}/src/Tracking/Blobs
  )
add_library(XVSeg
  ${SOURCES}
  )

# Install locally
file(COPY ${HEADERS} DESTINATION ${HEADER_OUTPUT_PATH})

# Install
