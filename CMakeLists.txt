
project(XVision2)
cmake_minimum_required(VERSION 2.8)

# Include custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
include(CheckIncludeFiles)
include(CheckLibraryExists)
find_package(PkgConfig)

# Find optional libraries

# Intel Performance Primitives
find_package(IPP)
if(IPP_FOUND)
  set(HAVE_IPP 1)
  include_directories(${IPP_INCLUDE_DIRS})
endif()

# OpenCV
pkg_check_modules(OPENCV REQUIRED opencv)
if(OPENCV_FOUND)
  include_directories(${OPENCV_INCLUDE_DIRS})
endif()

find_package(X11)
set(HAVE_LIBX11 ${X11_FOUND})

find_library(LIBXEXT Xext)
set(HAVE_LIBXEXT ${LIBXEXT_FOUND})

# SVS
if(ENV{SVS_PATH})
  set(SVS_PATH $ENV{SVS_PATH})
  set(HAVE_SVS 1)
  set(VIDERE_INCLUDE_DIR "${SVS_PATH}/src")
  set(VIDERE_LIB_DIR "${SVS_PATH}/bin")
  set(VIDERE_LIBRARIES svs svscalc svscap)
endif()

# Prosilica GigE
if(ENV{PVROOT})
  set(PVROOT $ENV{PVROOT})
  set(HAVE_PV 1)
  set(PROSILICA_INCLUDE_DIR "${PVROOT}/include")
  set(PROSILICA_LIB_DIR "${PVROOT}/lib/$ENV{PVARCH}")
endif()

# Libdc1394
check_include_files(dc1394/control.h HAVE_DC1394)
if(HAVE_DC1394)
  set(DVFILES "v2")
  set(DVFIRE_LIBS raw1394 dc1394)
endif()


# Video4Linux2
check_include_files(linux/videodev2.h HAVE_V4L2)

# Shared memory map
check_include_files(linux/shm.h HAVE_REMAP)

# System headers
check_include_files(fcntl.h sys/ioctl.h sys/time.h unistd.h)

# Generate config header
configure_file(
  ${PROJECT_SOURCE_DIR}/include/config.h.cmake.in 
  ${PROJECT_SOURCE_DIR}/include/config.h)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source (and modules)
add_subdirectory(src/drivers)
add_subdirectory(src/Tools)
add_subdirectory(src/Images)
add_subdirectory(src/Consoles)
add_subdirectory(src/Devices)
add_subdirectory(src/Tracking)
add_subdirectory(src/Segmentation)
# Broken add_subdirectory(src/frp)
