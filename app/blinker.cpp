#include "blinker.h"
#include "kernel/kernel.h"
#include "oslib/lcd.h"
#include "util/menu.h"

AppBlinker::AppBlinker() : AppBase() {
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

void AppBlinker::_menu_click(std::string label) {
    if (label == "ON") {
        _blink_on();
    } else if (label == "OFF") {
        _blink_off();
    }
}

void AppBlinker::_blink_on() {
    _blinking = true;
}

void AppBlinker::_blink_off() {
    _blinking = false;
    _led1->set_off();
}

void AppBlinker::on_foreground() {
    _menu = new LcdMenu(this, lcd);
    std::list<std::string> list;
    list.push_back("ON");
    list.push_back("OFF");
    _menu->set_labels(list, callback(this, &AppBlinker::_menu_click));
    _menu->start();
}

void AppBlinker::on_background() {
    _menu->stop();
    delete _menu;
}

void AppBlinker::release() {
    _blinking = false;
}
