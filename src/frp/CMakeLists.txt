
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# Library files
set(HEADERS
  Behavior.h
  Collectable.h
  Event.h
  Functoid.h
  Image.h
  Lambda.h
  Nclient.h
  NoCollectable.h
  Scout.h
  Task.h
  Track.h
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

execute_process(
  COMMAND "${EXECUTABLE_OUTPUT_PATH}/screendepth"
  OUTPUT_VARIALBE SCREEN_DEPTH)

add_definitions(-DSCREEN_DEPTH=${SCREEN_DEPTH})

pkg_check_modules(GLIB REQUIRED glib-2.0)
if(GLIB_FOUND)
  include_directories(${GLIB_INCLUDE_DIR})
  add_definitions(${GLIB_DEFINITIONS})
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/drivers/libdv
  )
add_library(frp
  ${SOURCES}
  )

add_dependencies(frp screendepth)

# Install locally
file(COPY ${HEADERS} DESTINATION ${HEADER_OUTPUT_PATH})

# Install
