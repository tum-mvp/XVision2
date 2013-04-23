
cmake_minimum_required(VERSION 2.8)

# Library files
set(HEADERS
  XVFeature.h
  XVGroupTracker.h
  XVGroupTracker.icc
  XVTracker.h
  XVTracker.icc
  Blobs/XVBlobFeature.h  
  Blobs/XVBlobs.h
  SSD/XVSSD.h
  Edges/XVPattern.h
  Edges/XVEdgeFeature.h
  Edges/XVPattern.icc
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

# Install locally
file(COPY ${HEADERS} DESTINATION ${HEADER_OUTPUT_PATH})

# Install
