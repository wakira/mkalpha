#ifndef UTIL_MENU_H_
#define UTIL_MENU_H_

#include <string>
#include <list>
#include <map>
#include "mbed.h"

class AppBase;
class Lcd;

class LcdMenu {
public:
    LcdMenu(AppBase *app, Lcd *lcd);
    void set_labels(std::list<std::string> list,
            Callback<void(std::string)> func);
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
    Callback<void(std::string)> _cb;
    std::map<int, std::string> _labels;
    AppBase *_app;
    Lcd *_lcd;
};

#endif
