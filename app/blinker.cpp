#include "blinker.h"
#include "kernel/kernel.h"
#include "oslib/lcd.h"
#include "util/menu.h"

void AppBlinker::AppBlinker() : AppBase() {
    _blinking = false;
}

void AppBlinker::run() {
    _led1 = (Led*) (Kernel::get_instance().request_device(this, DEVICE_LED1));
    call_every_ms(1000, callback(this, &AppBlinker::_blink));
}

void AppBlinker::_blink() {
    if (_blinking) {
        if (_led1->get_state() == 1) {
            _led1->set_off();
        } else {
            _led1->set_on();
        }
    }
}

void AppBlinker::_blink_on() {
    _blinking = true;
}

void AppBlinker::_blink_off() {
    _blinking = false;
}

void AppBlinker::on_foreground() {
    _menu = new LcdMenu(this, lcd);
    std::list<MenuEntry> list;
    list.push_back(MenuEntry("ON", callback(this, &AppBlinker::_blink_on)));
    list.push_back(MenuEntry("OFF", callback(this, &AppBlinker::_blink_off)));
    _menu->set_entries(list);
    _menu->start();
}

void AppBlinker::on_background() {
    _menu->stop();
    delete _menu;
}

void AppBlinker::release() {
    _blinking = false;
}
