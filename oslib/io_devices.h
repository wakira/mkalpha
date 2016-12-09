#ifndef OSLIB_IO_DEVICES_H_
#define OSLIB_IO_DEVICES_H_

class Device {
public:
    virtual ~Device() {}
};

enum IODevice {
    DEVICE_LED1,
    DEVICE_LED2,
    DEVICE_LED3,
    DEVICE_LED4
};

#endif
