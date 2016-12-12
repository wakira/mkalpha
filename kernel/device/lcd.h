#ifndef KERNEL_DEVICE_LCD_H_
#define KERNEL_DEVICE_LCD_H_

#include "oslib/lcd.h"
#include "mbed.h"

class LcdFactory;

class LcdDevice : public Lcd {
    friend LcdFactory;
public:
    virtual lcd_proxy operator->();
    virtual ~LcdDevice();
private:
    LcdDevice(AppBase *holder, LcdFactory *factory);
    C12832 *_c;
    AppBase *_holder;
    LcdFactory *_factory;
};

class LcdFactory {
public:
    LcdDevice* request_lcd_device(AppBase *holder);

    void release_lcd_device();

private:
    Mutex _mutex;
};

#endif
