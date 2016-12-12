#include "launcher.h"
#include "kernel/kernel.h"
#include "util/menu.h"
#include "oslib/lcd.h"
// applications
#include "app/blinker.h"

void AppLauncher::register_app(std::string name, AppBase *instance) {
    if (_app_list.count(name) == 0) {
        _app_list[name] = instance;
        _menu_entries.push_back(name);
    }
}

void AppLauncher::run() {
    Kernel::get_instance().put_foreground(this, this);
    // TODO write a real luancher
    // Kernel::get_instance().launch_app((*_app_list.begin()).second);
    // Kernel::get_instance().put_foreground(this, (*_app_list.begin()).second);
}

void AppLauncher::release() {}

void AppLauncher::_menu_click(std::string label) {
    Kernel::get_instance().launch_app(_app_list[label]);
    Kernel::get_instance().put_foreground(this, _app_list[label]);
}

void AppLauncher::on_foreground() {
    _menu = new LcdMenu(this, lcd);
    _menu->set_labels(_menu_entries, callback(this, &AppLauncher::_menu_click));
    _menu->start();
}

void AppLauncher::on_background() {
    _menu->stop();
}
