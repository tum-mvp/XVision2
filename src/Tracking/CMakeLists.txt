
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# Library files
set(HEADERS
  )

set(SOURCES
  Edges/XVEdgeFeature.cc
  Edges/XVPattern.cc
  Blobs/XVBlobFeature.cc
  SSD/XVSSD.cc
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Edges
  ${CMAKE_CURRENT_SOURCE_DIR}/Blobs
  ${CMAKE_CURRENT_SOURCE_DIR}/SSD
  ${PROJECT_SOURCE_DIR}/src/Tools
  ${PROJECT_SOURCE_DIR}/src/Images
  ${PROJECT_SOURCE_DIR}/src/Consoles
  ${PROJECT_SOURCE_DIR}/src/Devices
  ${PROJECT_SOURCE_DIR}/src/Segmentation
  )

add_library(XVTrack
  ${SOURCES}
  )

# This was disabled in the makefile
#add_subdirectory(Edges)
#add_subdirectory(Blobs)
#add_subdirectory(SSD)


# Install
