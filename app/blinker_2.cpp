#include "blinker_2.h"
#include "kernel/kernel.h"
#include "oslib/lcd.h"
#include "util/menu.h"

AppBlinker2::AppBlinker2() : AppBase() {
    _blinking = false;
}

void AppBlinker2::run() {
    _led1 = (Led*) (Kernel::get_instance().request_device(this, DEVICE_LED2));
    call_every_ms(1000, callback(this, &AppBlinker2::_blink));
}

void AppBlinker2::_blink() {
    if (_blinking) {
        if (_led1->get_state() == 1) {
            _led1->set_off();
        } else {
            _led1->set_on();
        }
    }
}

void AppBlinker2::_menu_click(std::string label) {
    if (label == "ON") {
        _blink_on();
    } else if (label == "OFF") {
        _blink_off();
    }
}

void AppBlinker2::_blink_on() {
    _blinking = true;
}

void AppBlinker2::_blink_off() {
    _blinking = false;
    _led1->set_off();
}

void AppBlinker2::on_foreground() {
    printf("BLINKER2 ON_FG\n");
    _menu = new LcdMenu(this, lcd);
    std::list<std::string> list;
    list.push_back("ON");
    list.push_back("OFF");
    _menu->set_labels(list, callback(this, &AppBlinker2::_menu_click));
    _menu->start();
}

void AppBlinker2::on_background() {
    printf("BLINKER2 ON_BG\n");
    _menu->stop();
    delete _menu;
}

void AppBlinker2::on_release() {
    _blink_off();
}
