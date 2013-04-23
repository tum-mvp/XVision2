
cmake_minimum_required(VERSION 2.8)

# Library files
set(HEADERS
  XVMatrix.h
  XVMatrix.icc
  XVList.h
  XVTools.h
  XVException.h
  XVMacros.h
  XVAffineWarp.h
  XVAffineWarp.icc
  XVGeometry.h
  XVOmniWarper.h
  XVRectRaster.h
  XVLookupTable.h
  XVLookupTable.icc
  camera_config.h
  XVStereoRectify.h
  XVStereoRectifyTypes.h
  )

set(SOURCES
  XVMatrix.cc
  XVAffineWarp.cc
  XVOmniWarper.cc
  XVGeometry.cc
  XVLookupTable.cc
  )

if(IPP_FOUND)
  set(SOURCES ${SOURCES} XVStereoRectify.cc)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/Images
  )
add_library(XVTools
  ${SOURCES}
  )

# Screendepth program
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
add_executable(screendepth sd.cc)
target_link_libraries(screendepth  
  ${X11_LIBRARIES})

# Install locally
file(COPY ${HEADERS} DESTINATION ${HEADER_OUTPUT_PATH})

# Install
