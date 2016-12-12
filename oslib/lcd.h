#ifndef OSLIB_LCD_H_
#define OSLIB_LCD_H_

#include "io_devices.h"
#include "app/base.h"
#include "C12832.h"
#include "mbed.h"


class lcd_proxy {
public:
    lcd_proxy(AppBase *holder, C12832 *c) {
        _holder = holder;
        _c = c;
        // _holder->lcd_mutex.lock();
    }
    ~lcd_proxy() {
        // _holder->lcd_mutex.unlock();
    }
    C12832* operator->() {
        // TODO check if foreground
        return _c;
    }
private:
    C12832 *_c;
    AppBase *_holder;
};

class Lcd : public Device {
public:
    virtual lcd_proxy operator->() = 0;
    virtual ~Lcd() {} // mbed has poor support for abstract class
};


#endif
