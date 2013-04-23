
cmake_minimum_required(VERSION 2.8)

# Library files
set(HEADERS
  )

set(SOURCES
  XVImageSeq.cc
  XVVideo.cc
  XVMpeg.cc
  )

# IPP
if(HAVE_IPP)
  set(SOURCES ${SOURCES} XVAVI.cc)
endif()

# SVS
if(HAVE_SVS)
  set(SOURCES ${SOURCES} Videre.cc StereoVidere.cc)
  include_directories(${VIDERE_INCLUDE_DIR})
  link_directories(${VIDERE_LIB_DIR})
  target_link_libraries(XVDevs ${VIDERE_LIBRARIES})
endif()

# Video4Linux2
if(HAVE_V4L2)
  set(SOURCE ${SOURCES} XVV4L2.cc)
endif()

# Prosilica
if(HAVE_PV)
  include_directories(${PROSILICA_INCLUDE_DIR})
  set(SOURCES ${SOURCES} XVPv.cc)
endif()

# libdc1394
if(HAVE_DC1394)
  set(HEADERS ${HEADERS} XVDig1394.h)
  set(SOURCES ${SOURCES} XVDig1394.cc XVFlea2G.cc)
endif()


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/drivers/mpeg_lib-1.3.1
  ${PROJECT_SOURCE_DIR}/src/Images
  ${PROJECT_SOURCE_DIR}/src/Tools
  )

add_library(XVDevs
  ${SOURCES}
  )

# Install
