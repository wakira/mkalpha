#ifndef OSLIB_IO_EVENTS_H_
#define OSLIB_IO_EVENTS_H_

enum IOEvent {
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_FIRE,
    WEBSOCKET_IN, // wifi
    JOYSTICK_LONG_PRESS, // not registerble
    JOYSTICK_LONGER_PRESS // not registerble
};

#endif
