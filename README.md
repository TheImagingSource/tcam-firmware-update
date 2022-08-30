

# What is this

This tool allows you to update the firmware of your TIS USB camera under Linux.

# Usage

### examples

List all available cameras:

```
tcam-firmware-update -l
```

Get information about a single camera:

```
tcam-firmware-update -id <serial number>
```

Apply firmware

```
tcam-firmware-update -ud <serial number> -f firmware-file
```
It is recommended to have only one camera attached to the system when updating the firmware.

Switch camera to UVC/proprietary mode

```
tcam-firmware-update -d <serialnumber> -m uvc
tcam-firmware-update -d <serialnumber> -m proprietary
```

Switching to uvc mode is only necessary for USB2 cameras.

# Firmware Files

# Usb2

Firmware file for usb-2.0 cameras are located in data/firmware/usb2

# Usb3

If you need firmware files for USB3 cameras,
please send us a request.

http://www.theimagingsource.com/en_US/company/contact/

# Building

## Dependencies

Required dependencies are:

- libusb-1.0
- libzip
- libudev

For compilation additional dev packages may be required.

## Compiling

```
mkdir tcam-firmware-update/build
cd tcam-firmware-update/build
cmake ..
make
```

Do not call `make install`.
tcam-firmware-update is not intended for system wide use.


# License

tcam-firmware-update is released under the Apache 2.0 license.

This project includes:
- 7z, which is published as public domain.
- PugiXml 1.6, which is available under the "MIT" license.
