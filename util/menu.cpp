#include "menu.h"
#include "kernel/kernel.h"
#include "oslib/io_events.h"
#include "oslib/lcd.h"
#include "C12832.h"

LcdMenu::LcdMenu(AppBase *app, Lcd *lcd) {
    _app = app;
    _lcd = lcd;
    _cur_pos = 0;
    _cnt = 0;
    _running = false;
}

void LcdMenu::set_labels(std::list<std::string> list,
        Callback<void(std::string)> cb) {
    _labels.clear();
    _cb = cb;
    int old_cnt = _cnt;
    _cnt = 0;
    for(std::list<std::string>::iterator it = list.begin();
            it != list.end(); it++) {
        _cnt++;
        _labels[_cnt] = (*it);
    }
    if (_cnt != old_cnt) {
        _cur_pos = 1;
    }
    if (_running) {
        _draw();
    }
}

void LcdMenu::start() {
    _running = true;
    Kernel &k = Kernel::get_instance();
    k.register_io_event_handler(_app, JOYSTICK_UP,
            callback(this, &LcdMenu::_joystick_up_handler));
    k.register_io_event_handler(_app, JOYSTICK_DOWN,
            callback(this, &LcdMenu::_joystick_down_handler));
    k.register_io_event_handler(_app, JOYSTICK_FIRE,
            callback(this, &LcdMenu::_joystick_fire_handler));
    // show menu
    _app->lcd_mutex.lock();
    (*_lcd)->cls();
    _app->lcd_mutex.unlock();
    if (_cnt != 0) {
        _cur_pos = 1;
        _draw();
    }
}

void LcdMenu::_draw() {
    _app->lcd_mutex.lock();
    (*_lcd)->cls();
    (*_lcd)->locate(0,0);
    // draw previous line (view_pos - 1)
    if (_cur_pos - 1 > 0) {
        (*_lcd)->printf("   %s\n", _labels[_cur_pos - 1].c_str());
    } else {
        (*_lcd)->printf("\n");
    }
    // draw current line (view_pos)
    (*_lcd)->printf("x  %s\n", _labels[_cur_pos].c_str());
    // draw next line (view_pos)
    if (_cur_pos + 1 <= _cnt) {
        (*_lcd)->printf("   %s", _labels[_cur_pos + 1].c_str());
    }
    _app->lcd_mutex.unlock();
}

void LcdMenu::stop() {
    _running = false;
    Kernel &k = Kernel::get_instance();
    k.unregister_io_event_handler(_app, JOYSTICK_UP);
    k.unregister_io_event_handler(_app, JOYSTICK_DOWN);
    k.unregister_io_event_handler(_app, JOYSTICK_FIRE);
}

void LcdMenu::_joystick_up_handler() {
    _cur_pos--;
    if (_cur_pos == 0) {
        _cur_pos = _cnt;
    }
    if (_running) {
        _draw();
    }
}

void LcdMenu::_joystick_down_handler() {
    _cur_pos++;
    if (_cur_pos > _cnt) {
        _cur_pos = 1;
    }
    if (_running) {
        _draw();
    }
}

void LcdMenu::_joystick_fire_handler() {
    // _app->call(_items[_cur_pos].func);
    _app->call(_cb, _labels[_cur_pos]);
}
