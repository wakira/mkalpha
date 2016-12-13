#ifndef APP_BLINKER2_H_
#define APP_BLINKER2_H_

#include "base.h"
#include "oslib/led.h"
#include <string>

class LcdMenu;

class AppBlinker2 : public AppBase {
public:
    AppBlinker2();
protected:
    virtual void run();
    virtual void release();
    virtual void on_foreground();
    virtual void on_background();
private:
    void _menu_click(std::string label);
    void _blink_on();
    void _blink_off();
    bool _blinking;
    Led *_led1;
    void _blink();
    LcdMenu *_menu;
};

#endif
