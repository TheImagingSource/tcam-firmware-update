/*
 * Copyright 2014 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <cstdint>

namespace tis
{

#define TIS_VENDOR_ID 0x199e


// Flag for UVC/Proprietary switch
// only relevant for USB2 cameras
// all other cameras always have UVC enabled
enum UVC_COMPLIANCE
{
    CAMERA_INTERFACE_MODE_PROPRIETARY,
    CAMERA_INTERFACE_MODE_UVC,
};


// TYPE identifies the kind of firmware writing
// operation we need to perform,
// NOT the type of camera series.
enum TYPE
{
    UNKNOWN = 0,
    USB2,  // One4All, 72, 42, 22
    USB3,  // 23 series
    USB33, // 33, 37, 38, 32
};


struct camera_type
{
    TYPE camera_type;
    uint32_t idVendor;
    uint32_t idProduct;
    const char* product_name;
};

} /* namespace tis */

#endif /* _DEFINITIONS_H_ */
