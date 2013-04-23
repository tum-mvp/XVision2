
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

include(ExternalProject)

ExternalProject_Add(
  mpeg_lib-1-3-1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mpeg_lib-1.3.1"
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND "./configure" "--prefix=${EXTERNAL_PROJECT_INSTALL_PATH}"
  BUILD_COMMAND make
  INSTALL_COMMAND make install
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

# Note this is broken / out-of-date
#ExternalProject_Add(
  #meteor-1-5-4
  #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/meteor-1.5.4"
  #BUILD_IN_SOURCE 1
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND make
  #INSTALL_COMMAND ""
  #)

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
