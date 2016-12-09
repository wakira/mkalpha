#include "blinker.h"
#include "kernel/kernel.h"

void AppBlinker::run() {
    _led1 = static_cast<Led*> (Kernel::get_instance().request_device(this, DEVICE_LED1));
    call_every_ms(1000, callback(this, &AppBlinker::_blink));
}

void AppBlinker::_blink() {
    if (_led1->get_state() == 1) {
        _led1->set_off();
    } else {
        _led1->set_on();
    }
}

void AppBlinker::release() {
    // stop blinking

}
