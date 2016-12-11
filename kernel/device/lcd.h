#ifndef KERNEL_DEVICE_LCD_H_
#define KERNEL_DEVICE_LCD_H_

#include "oslib/lcd.h"

class LcdDevice : public Lcd {
public:
    LcdDevice(AppBase *holder);
    virtual lcd_proxy operator->();
    ~LcdDevice();
private:
    C12832 *_c;
    AppBase *_holder;
};

#endif
