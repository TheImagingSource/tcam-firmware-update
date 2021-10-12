# Copyright 2021 The Imaging Source Europe GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 3.2)

#Name your project here
project(tcam-firmware-update)


IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "/usr/" CACHE PATH "Common prefix for all installed files." FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(TCAM_FW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TCAM_FW_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TCAM_FW_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TCAM_FW_BINARY_DIR}/lib)

find_package(libusb-1.0 REQUIRED QUIET)
find_package(LibZip REQUIRED QUIET)

add_subdirectory(external/PugiXml EXCLUDE_FROM_ALL)

add_subdirectory(src)
