#include "kernel/device/lcd.h"
#include "C12832.h"
#include "mbed.h"

LcdDevice::LcdDevice(AppBase *holder) {
    _holder = holder;
    _c = new C12832(p5, p7, p6, p8, p11);
}

lcd_proxy LcdDevice::operator->() {
    return lcd_proxy(_holder, _c);
}

LcdDevice::~LcdDevice() {
    delete _c;
}
