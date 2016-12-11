#ifndef OSLIB_LCD_H_
#define OSLIB_LCD_H_

#include "io_devices.h"

class C12832;

class lcd_proxy {
public:
    lcd_proxy(AppBase *holder, C12832 *c) {
        _holder = holder;
        _holder->lcd_mutex.lock();
    }
    ~lcd_proxy() {
        _holder->lcd_mutex.unlock();
    }
    C12832* operator->() {
        return _c;
    }
private:
    C12832 *_c;
    AppBase *_holder;
};

class Lcd : public Device {
public:
    virtual lcd_proxy operator->();
};


#endif
