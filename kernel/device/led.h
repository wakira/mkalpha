#ifndef KERNEL_DEVICE_LED_H_
#define KERNEL_DEVICE_LED_H_

#include "oslib/led.h"
#include "oslib/io_devices.h"
#include "mbed.h"

class LedDevice : public Led {
public:
    LedDevice(IODevice id);
    virtual void set_on();
    virtual void set_off();
    virtual int get_state();
    ~LedDevice();
private:
    DigitalOut *_led;
};

#endif
