#ifndef UTIL_MENU_H_
#define UTIL_MENU_H_

#include "app/base.h"
#include "oslib/lcd.h"
#include <string>
#include <list>
#include "mbed.h"

struct MenuEntry {
    MenuEntry(std::string label, Callback<void()> func) {
        this->label = label;
        this->func = func;
    }

    std::string label;
    Callback<void()> func;
};

class LcdMenu {
public:
    LcdMenu(AppBase *app, Lcd *lcd);
    void set_entries(std::list<MenuEntry> list);
    void start();
    void stop();
private:
    // io event handlers
    void _joystick_up_handler();
    void _joystick_down_handler();
    void _joystick_fire_handler();

    void _draw();

    bool _running;
    int _cnt;
    int _cur_pos;
    std::map<int, MenuEntry> _items;
    AppBase *_app;
    Lcd *_lcd;
};

#endif
