#ifndef APP_BLINKER_H_
#define APP_BLINKER_H_

#include "base.h"
#include "oslib/led.h"
#include "util/menu.h"

class AppBlinker : public AppBase {
public:
    AppBlinker();
    virtual void run();
    virtual void release();
    virtual void on_foreground();
    virtual void on_background();
private:
    bool _blinking;
    Led *_led1;
    void _blink();
    LcdMenu *_menu;
};

#endif
