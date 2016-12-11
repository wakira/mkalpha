#ifndef APP_BLINKER_H_
#define APP_BLINKER_H_

#include "base.h"
#include "oslib/led.h"

class LcdMenu;

class AppBlinker : public AppBase {
public:
    AppBlinker();
    virtual void run();
    virtual void release();
    virtual void on_foreground();
    virtual void on_background();
private:
    void _blink_on();
    void _blink_off();
    bool _blinking;
    Led *_led1;
    void _blink();
    LcdMenu *_menu;
};

#endif
