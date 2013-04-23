
project(xvision2)
cmake_minimum_required(VERSION 2.8)

# Include custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
include(CheckIncludeFiles)
include(CheckLibraryExists)
find_package(PkgConfig)

# Find optional libraries

# Intel Performance Primitives
option(USE_IPP "Use Intel Performance Primitives" ON)
find_package(IPP)
if(IPP_FOUND AND USE_IPP)
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
  set(HAVE_DV 1)
  set(HAVE_VIDEO1394_NEW 1)

  find_path(VIDE01394_DIR "/dev/video1394/0")
  set(HAVE_VIDEO1394_DIR ${VIDE01394_DIR_FOUND})
endif()

# Pthreads
check_library_exists(pthread pthread_create "" HAVE_LIBPTHREAD)

# X Libs
check_library_exists(X11 XOpenDisplay "" HAVE_LIBX11)
check_library_exists(Xext XShmAttach "" HAVE_LIBXEXT)
check_library_exists(Xxf86dga XF86DGAQueryExtension "" HAVE_LIBXXF86DGA)
check_library_exists(Xxf86vm XF86VidModeQueryExtension "" HAVE_LIBXXF86VM)

check_library_exists(png png_get_copyright "" HAVE_LIBPNG)

# Video4Linux2
check_include_files(linux/videodev2.h HAVE_V4L2)

# Shared memory map
check_include_files(linux/shm.h HAVE_REMAP)

# System headers
check_include_files(fcntl.h HAVE_FCNTL_H)
check_include_files(sys/ioctl.h HAVE_SYS_IOCTL_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(getpagesize HAVE_GETPAGESIZE)

# Catkin support
option(USE_CATKIN "Use the Catkin meta-buildsystem" ON)
if(USE_CATKIN)

  set(XVISION2_LIBS XVTools XVImages XVSeg XVTrack XVDevs XVCons)

  # Get the information about this package's buildtime dependencies
  find_package(catkin REQUIRED)
  # Declare this catkin package's runtime dependencies 
  catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${XVISION2_LIBS}
    )

  set(EXTERNAL_PROJECT_INSTALL_PATH ${CATKIN_DEVEL_PREFIX})
else()
  # Set the lib and bin output dirs
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
  set(EXTERNAL_PROJECT_INSTALL_PATH ${PROJECT_SOURCE_DIR})
endif()

# Generate config header
configure_file(
  ${PROJECT_SOURCE_DIR}/include/config.h.cmake.in 
  ${PROJECT_SOURCE_DIR}/include/config.h)

set(HEADER_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/include)
include_directories(${HEADER_OUTPUT_PATH})

# Add source (and modules)
add_subdirectory(src/drivers)
add_subdirectory(src/Tools)
add_subdirectory(src/Images)
add_subdirectory(src/Consoles)
add_subdirectory(src/Devices)
add_subdirectory(src/Tracking)
add_subdirectory(src/Segmentation)
# Broken add_subdirectory(src/frp)

