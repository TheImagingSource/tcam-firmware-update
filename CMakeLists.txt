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

set( default_build_type "Release" )

# Set a default build type if none was specified
# Override the default by specifying default_build_type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  if( NOT default_build_type )
    set( default_build_type "RelWithDebInfo" )
  endif()
  message(
    STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE
    ${default_build_type}
    CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
else()
  message( STATUS "Current CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" )
endif()

set(TCAM_FW_VERSION_MAJOR 1)
set(TCAM_FW_VERSION_MINOR 0)
set(TCAM_FW_VERSION_PATCH 0)

set(TCAM_FW_VERSION "${TCAM_FW_VERSION_MAJOR}.${TCAM_FW_VERSION_MINOR}.${TCAM_FW_VERSION_PATCH}" CACHE STRING "Version number")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/" CACHE PATH "Common prefix for all installed files." FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_INSTALL_PREFIX "/")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_compile_options( -Wno-psabi )   # disable ABI change message of g++8
add_compile_options( -Wall -Wextra -pedantic -pthread )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

set(TCAM_FW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TCAM_FW_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TCAM_FW_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TCAM_FW_BINARY_DIR}/lib)

find_package(libusb-1.0 REQUIRED)
find_package(LibZip REQUIRED)

add_subdirectory(external/PugiXml EXCLUDE_FROM_ALL)

add_subdirectory(src)

install(DIRECTORY firmware DESTINATION ${CMAKE_INSTALL_PREFIX})

include(CPackComponent)

set(CPACK_SET_DESTDIR "on")
set(CPACK_GENERATOR "TGZ")

set(CPACK_PACKAGE_VERSION_MAJOR "${TCAM_FW_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TCAM_FW_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TCAM_FW_VERSION_PATCH}")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

include(package-name)

set(CPACK_PACKAGE_NAME "tcam-firmware-upload")
create_package_name(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}" "${TCAM_FW_VERSION}")


# always last
include(CPack)
