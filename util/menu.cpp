#include "menu.h"
#include "kernel/kernel.h"
#include "oslib/io_events.h"
#include "oslib/lcd.h"
#include "C12832.h"

LcdMenu::LcdMenu(AppBase *app, Lcd *lcd) {
    _app = app;
    _lcd = lcd;
    _cur_pos = 0;
    _running = false;
}

void LcdMenu::set_entries(std::list<MenuEntry> list) {
    _items.clear();
    old_cnt = _cnt;
    _cnt = 0;
    for(std::list<MenuEntry>::iterator it = list.begin();
            it != list.end(); list++) {
        _cnt++;
        _items[_cnt] = (*it);
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
    k.register_io_handler(_app, JOYSTICK_UP,
            callback(this, LcdMenu::_joystick_up_handler));
    k.register_io_handler(_app, JOYSTICK_DOWN,
            callback(this, LcdMenu::_joystick_down_handler));
    k.register_io_handler(_app, JOYSTICK_FIRE,
            callback(this, LcdMenu::_joystick_fire_handler));
    // show menu
    (*lcd)->cls();
    if (_cnt != 0) {
        _cur_pos = 1;
        _draw(1);
    }
}

void LcdMenu::_draw() {
    (*lcd)->cls();
    // draw previous line (view_pos - 1)
    if (_cur_pos - 1 > 0) {
        (*lcd)->locate(1,1);
        (*lcd)->printf("%s", _items[_cur_pos - 1].label.c_str());
    }
    // draw current line (view_pos)
    (*lcd)->locate(0,2);
    printf("*");
    (*lcd)->locate(1,2);
    (*lcd)->printf("%s", _items[_cur_pos].label.c_str());
    // draw next line (view_pos)
    if (_cur_pos + 1 < _cnt) {
        (*lcd)->locate(1,3);
        (*lcd)->printf("%s", _items[_cur_pos + 1].label.c_str());
    }
}

void LcdMenu::stop() {
    _running = false;
    Kernel &k = Kernel::get_instance();
    k.unregister_io_handler(_app, JOYSTICK_UP);
    k.unregister_io_handler(_app, JOYSTICK_DOWN);
    k.unregister_io_handler(_app, JOYSTICK_FIRE);
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
    _app->call(_items[cur_pos].func);
}
