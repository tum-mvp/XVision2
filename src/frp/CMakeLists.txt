
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# Library files
set(HEADERS
  )

set(SOURCES
  Collectable.cc
  Behavior.cc
  Event.cc
  Image.cc
  Task.cc
  Track.cc
  Scout.cc
  Nclient.c
  )


add_definitions(-DSCREEN_DEPTH=32)

pkg_check_modules(GLIB REQUIRED glib-2.0)
if(GLIB_FOUND)
  include_directories(${GLIB_INCLUDE_DIR})
  add_definitions(${GLIB_DEFINITIONS})
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
add_library(frp
  ${SOURCES}
  )

# Install
