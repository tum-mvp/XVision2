
cmake_minimum_required(VERSION 2.8)

# Library files
set(HEADERS
  XVWindow.h
  XVDrawable.h
  XVInteractive.h
  XVWindowX.h
  XVRemoteWindowX.h
  )

set(SOURCES
  XVWindowX.cc
  XVRemoteWindowX.cc
  )

find_package(X11 REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/Tools
  ${PROJECT_SOURCE_DIR}/src/Images
  ${PROJECT_SOURCE_DIR}/src/Devices
  ${X11_INCLUDE_DIR}
  )
add_library(XVCons
  ${SOURCES}
  )

# Install locally
file(COPY ${HEADERS} DESTINATION ${HEADER_OUTPUT_PATH})

# Install
