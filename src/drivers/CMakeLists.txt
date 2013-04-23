
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

include(ExternalProject)

ExternalProject_Add(
  mpeg_lib-1-3-1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mpeg_lib-1.3.1"
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND "./configure"
  BUILD_COMMAND make
  INSTALL_COMMAND ""
  )

ExternalProject_Add(
  tiff-3-5-5
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tiff-v3.5.5"
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make
  INSTALL_COMMAND ""
  )

ExternalProject_Add(
  jpeg-6b
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jpeg-6b"
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make
  INSTALL_COMMAND ""
  )

# Note this is broken
#ExternalProject_Add(
  #libdv
  #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libdv"
  #BUILD_IN_SOURCE 1
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND make
  #INSTALL_COMMAND ""
  #)

# Install
