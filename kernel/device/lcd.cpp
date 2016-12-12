#include "kernel/device/lcd.h"
#include "C12832.h"
#include "mbed.h"
#include "rtos.h"

LcdDevice::LcdDevice(AppBase *holder, LcdFactory *factory) {
    _holder = holder;
    _factory = factory;
    _c = new C12832(p5, p7, p6, p8, p11);
}

lcd_proxy LcdDevice::operator->() {
    return lcd_proxy(_holder, _c);
}

LcdDevice::~LcdDevice() {
    delete _c;
    // tell LCD factory that lcd is released
    _factory->release_lcd_device();

}

LcdDevice* LcdFactory::request_lcd_device(AppBase *holder) {
    printf("%x obtain lcd lock\n", Thread::gettid());
    _mutex.lock();
    printf("lcd lock success\n");
    LcdDevice *ret = new LcdDevice(holder, this);
    return ret;
}

void LcdFactory::release_lcd_device() {
    _mutex.unlock();
    printf("%x lcd release\n", Thread::gettid());
}

