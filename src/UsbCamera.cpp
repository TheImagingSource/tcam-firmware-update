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

#include "UsbCamera.h"

#include <cstring>
#include <stdexcept>

namespace tis
{

// lookup table for all supported camera types
static const camera_type camera_type_list[] = {
    { UNKNOWN, 0, 0, "" },
    /* registration of USB2 cameras with jumper attached */
    { USB2, 0xeb1a, 0x2760, "Empia" },
    { USB3, TIS_VENDOR_ID, 0x8666, "DFK AFU 130" },
    { USB2, TIS_VENDOR_ID, 0x8202, "DFK 22xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8302, "DFK 22xUC02 UVC" },
    { USB2, TIS_VENDOR_ID, 0x8203, "DFK 61xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8204, "DFK 41xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8205, "DFx 51xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8206, "DFx 41xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8207, "DFK 72xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8307, "DFK 72xUC02 UVC" },
    { USB2, TIS_VENDOR_ID, 0x8208, "DFK 42xUC02" },
    { USB2, TIS_VENDOR_ID, 0x8308, "DFK 42xUC02 UVC" },

    // { DBG_TEXT("BF-532"), DBG_TEXT("Dxx AU/BU"), eUSBCAM, 0x8101, &CTIS_AU_Camera_create },
    // { DBG_TEXT("BF-532"), DBG_TEXT("DxK 51xU03"), eEUVCCAM, 0x8102, &create_cam_ccd_51 },
    // { DBG_TEXT("MT9V023"), DBG_TEXT("DxK 21AUC02"), eEUVCCAM, 0x8201, &CDevice_MT9V023_create },
    // { DBG_TEXT("MT9V023"), DBG_TEXT("DxK 21AUC02"), eEUVCCAM, 0x8202, &CDevice_MT9V023_create },

    { USB3, 0x04b4, 0x00f3, "Westbridge" },
    { USB3, TIS_VENDOR_ID, 0x8401, "DMK 23U618" },
    { USB3, TIS_VENDOR_ID, 0x8402, "DMK 23U445" },
    { USB3, TIS_VENDOR_ID, 0x8403, "DMK 23U274" },
    { USB3, TIS_VENDOR_ID, 0x8404, "DMK 23UV024" },
    { USB3, TIS_VENDOR_ID, 0x8405, "DMK 23UM021" },
    { USB3, TIS_VENDOR_ID, 0x8406, "DMK 23UP031" },
    { USB3, TIS_VENDOR_ID, 0x8411, "DFK 23U618" },
    { USB3, TIS_VENDOR_ID, 0x8412, "DFK 23U445" },
    { USB3, TIS_VENDOR_ID, 0x8413, "DFK 23U274" },
    { USB3, TIS_VENDOR_ID, 0x8414, "DFK 23UV024" },
    { USB3, TIS_VENDOR_ID, 0x8415, "DFK 23UM021" },
    { USB3, TIS_VENDOR_ID, 0x8416, "DFK 23UP031" },
    { USB3, TIS_VENDOR_ID, 0x8426, "DMK 23UP031-AF" },
    { USB3, TIS_VENDOR_ID, 0x8436, "DFK 23UP031-AF" },
    { USB37, TIS_VENDOR_ID, 0x9402, "DMK 37BUX178" }
};


const camera_type find_camera_type(const unsigned int& idVendor, const unsigned int& idProduct)
{
    for (auto& c : camera_type_list)
    {
        if (idVendor == c.idVendor && idProduct == c.idProduct)
        {
            return c;
        }
    }
    const unsigned int idProductMask = 0xFC00;

    // type not known assure we still can handle the device
    // as long as it is one of ours
    if (idVendor != 0x199e)
        return camera_type_list[0];

    if (idProductMask == 0x9400)
    {
        return { USB37, idVendor, idProduct, "Unkown USB37 Camera" };
    }
    else if (idProductMask == 0x9000 ||
             idProductMask == 0x9800 ||
             idProductMask == 0x9c00)
    {
        return { USB33, idVendor, idProduct, "Unkown USB33 Camera" };
    }
    else if (idProductMask == 0x8200 || idProductMask == 0x8300)
    {
        return { USB2, idVendor, idProduct, "Unknown USB2 Camera" };
    }
    else if (idProductMask == 0x8400 ||
             idProductMask == 0x8500 ||
             idProductMask == 0x8600 ||
             idProductMask == 0x8700)
    {
        return { USB2, idVendor, idProduct, "Unknown USB2.3 Camera" };
    }

    return camera_type_list[0];
}


UsbCamera::UsbCamera(std::shared_ptr<UsbSession> session, device_info _dev, unsigned int _interface)
    : usb_session(session), dev_handle(nullptr), dev(_dev), interface(_interface)
{
}


UsbCamera::~UsbCamera()
{
    close();
}


bool UsbCamera::open()
{
    try
    {
        // List all devices.
        libusb_device** devs;
        int cnt = libusb_get_device_list(usb_session->get_session(), &devs);
        if (cnt < 0)
        {
            throw std::runtime_error("Unable to retrieve device list. " + std::to_string(cnt));
        }

        // Look for the device with matching serial number and store its device
        // handle.
        for (ssize_t i = 0; i < cnt; i++)
        {
            // Get device descriptor for current device in list.
            libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(devs[i], &desc);
            if (r < 0)
            {
                throw std::runtime_error("Unable to retrieve device descriptor. "
                                         + std::to_string(r));
            }

            // Ignore device if not from TIS or otherwise needed.
            if (desc.idVendor != 0x199e && desc.idVendor != 0xeb1a && desc.idVendor != 0x04b4)
            {
                continue;
            }
            // Get device handle.
            libusb_device_handle* dh;
            r = libusb_open(devs[i], &dh);
            if (r < 0)
            {
                throw std::runtime_error("Unable to open device. " + std::to_string(r));
            }

            // Get ASCII representation of device serial number.
            char serial[256];
            r = libusb_get_string_descriptor_ascii(dh,
                                                   desc.iSerialNumber,
                                                   (unsigned char*)serial,
                                                   sizeof(serial));
            if (r < 0)
            {
                throw std::runtime_error(
                    "Unable to get ASCII representation of device serial number. "
                    + std::to_string(r));
            }

            // Ignore device if it does not have the expected serial number.
            if (strcmp(serial, dev.serial) != 0)
            {
                // Close device and continue to next device in list.
                libusb_close(dh);
                continue;
            }

            // Device handle of desired camera has been retrieved successfully, assign
            // to member variable and break loop.
            this->dev_handle = dh;
            break;
        }
        // Free device list.
        libusb_free_device_list(devs, 1);

        if (this->dev_handle == NULL)
        {
            throw std::runtime_error("Unable to attain device handle.");
        }

        claim_interface();
    }
    catch (std::runtime_error& err)
    {
        throw;
    }
    return true;
}


bool UsbCamera::close()
{
    if (this->dev_handle != nullptr)
    {
        release_interface();
        libusb_close(this->dev_handle);
        this->dev_handle = nullptr;
    }
    return true;
}


device_info UsbCamera::get_device_info()
{
    return dev;
}


camera_type UsbCamera::get_camera_type()
{
    return find_camera_type(this->dev.idVendor, this->dev.idProduct);
}


void UsbCamera::claim_interface()
{
    if (libusb_kernel_driver_active(this->dev_handle, this->interface) == 1)
    {
        if (libusb_detach_kernel_driver(dev_handle, interface) != 0)
        {
            throw std::runtime_error("Unable to detach kernel driver.");
        }
    }

    int r = libusb_claim_interface(this->dev_handle, this->interface);
    if (r < 0)
    {
        throw std::runtime_error("Unable to claim interface.");
    }
}


void UsbCamera::release_interface()
{
    int r = libusb_release_interface(this->dev_handle, this->interface);
    if (r != 0)
    {
        throw std::runtime_error("Unable to release interface.");
    }
    libusb_attach_kernel_driver(this->dev_handle, this->interface);
}

} /* namespace tis */
