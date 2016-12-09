#include "kernel/device/led.h"

LedDevice::LedDevice(IODevice id) {
    switch (id) {
        case DEVICE_LED1:
            _led = new DigitalOut(LED1);
            break;
        case DEVICE_LED2:
            _led = new DigitalOut(LED2);
            break;
        case DEVICE_LED3:
            _led = new DigitalOut(LED3);
            break;
        case DEVICE_LED4:
            _led = new DigitalOut(LED4);
            break;
        default:
            _led = 0; // TODO should instead throw exception
    }
}

void LedDevice::set_on() {
    _led->write(1);
}

void LedDevice::set_off() {
    _led->write(0);
}

int LedDevice::get_state() {
    return _led->read();
}

LedDevice::~LedDevice() {
    delete _led;
}
