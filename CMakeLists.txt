# Copyright 2018 The Flutter Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

cmake_minimum_required(VERSION 3.5.2)

project(flutter_essos)

set(FLUTTER_ENGINE_SHA b9523318caa1a99ffde8adaf331212eb879cabc9)

set(FLUTTER_EMBEDDER_ARTIFACTS_ZIP ${CMAKE_BINARY_DIR}/flutter_embedder_${FLUTTER_ENGINE_SHA}.zip)
set(FLUTTER_ARTIFACTS_ZIP          ${CMAKE_BINARY_DIR}/flutter_artifact_${FLUTTER_ENGINE_SHA}.zip)
set(FLUTTER_BUCKET_BASE "https://storage.googleapis.com/flutter_infra/flutter")
set(CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}/cmake")


find_package(PkgConfig)
pkg_check_modules(EGL            REQUIRED egl)

# Executable
file(GLOB_RECURSE FLUTTER_ESSOS_SRC
  "src/*.cc"
  "src/*.cpp"
  "src/*.h"
)

link_directories(${CMAKE_BINARY_DIR})

add_executable(flutter_essos ${FLUTTER_ESSOS_SRC})

target_link_libraries(flutter_essos
  ${EGL_LIBRARIES}
  flutter_engine
  essos
  dl
)

target_include_directories(flutter_essos
  PRIVATE
  ${EGL_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
)
