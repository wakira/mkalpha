#ifndef OSLIB_LED_H_
#define OSLIB_LED_H_

#include "io_devices.h"

class Led : public Device {
public:
    virtual void set_on() = 0;
    virtual void set_off() = 0;
    virtual int get_state() = 0;
};

#endif
