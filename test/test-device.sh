#! /bin/sh

exec $1 $(ls /dev/serial/by-id/usb-Texas_Instruments_TI_CC2540_USB_CDC_*)
