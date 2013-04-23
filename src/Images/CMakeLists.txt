
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# Library files
set(HEADERS
  XVImageBase.h
  XVImageRGB.h
  XVImageRGB.icc
  XVImageScalar.h
  XVImageIterator.h
  XVImageFilters.icc
  XVPixel.h
  XVColorImage.h
  XVImageYUV.h
  XVImageYUV.icc
  XVImageYUV422.h
  XVImageBase.icc
  XVImageScalar.icc
  XVImageHSV.h
  XVImageIO.h
  XVImageIO.icc
  XVImageYCbCr.h
  XVMaskedImage.h
  )

set(SOURCES
  XVColorBase.cc
  XVImageHSV.cc
  XVImageScalar.cc
  XVImageYUV422.cc
  XVMaskedImage.cc
  XVImageBase.cc
  XVImageRGB.cc
  XVImageYCbCr.cc
  XVImageYUV.cc
  XVPixel.cc
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/Tools
  )
add_library(XVImages
  ${SOURCES}
  )

# Install
