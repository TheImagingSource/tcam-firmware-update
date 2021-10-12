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

add_subdirectory(33u)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(fwu-files
  FileHandling.cpp
  firmware-update.cpp
  UsbCamera.cpp
  Usb2Camera.cpp
  Usb3Camera.cpp
  Usb33Camera.cpp
  UsbHandler.cpp
  UsbSession.cpp
  FileHandling.cpp)

add_executable(tcam-firmware-update ${fwu-files} $<TARGET_OBJECTS:33update> $<TARGET_OBJECTS:pugixml_obj>)

target_include_directories(tcam-firmware-update
  PRIVATE ${LIBUSB_1_INCLUDE_DIRS}
  PRIVATE ${LIBZIP_INCLUDE_DIR}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/PugiXml")

target_link_libraries(tcam-firmware-update ${LIBUSB_1_LIBRARIES} ${LIBZIP_LIBRARY})

install(TARGETS tcam-firmware-update
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
