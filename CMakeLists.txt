
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 3.2.2)

project(react-native-desktop)

set(PROJECT_NAME "react-native-desktop")
set(PROJECT_VERSION "0.0.1")

include_directories(./React/Layout)
enable_testing()

message(STATUS "External modules dirs: ${EXTERNAL_MODULES_DIR}")

#add_subdirectory(ExternalTestModule)
#add_subdirectory(ExternalTestModule2)

foreach(loop_var ${EXTERNAL_MODULES_DIR})
  message(STATUS "loop_var: ${loop_var}")
  add_subdirectory(${loop_var})
endforeach(loop_var)

add_subdirectory(ReactQt)
add_subdirectory(Examples)
add_subdirectory(RNTester)

add_custom_target(
  "watch"
  COMMAND node -e \"require('fs').watch('${CMAKE_SOURCE_DIR}/ReactQt',function(e,f){require('child_process').spawn('make',{stdio:'inherit'})})\"
  DEPENDS react-native
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
