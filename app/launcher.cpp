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
}

void AppLauncher::release() {}

void AppLauncher::_menu_click(std::string label) {
    AppBase *instance = _app_list[label];
    if (!instance->is_running()) {
        Kernel::get_instance().launch_app(instance);
    }
    Kernel::get_instance().put_foreground(this, instance);
}

void AppLauncher::on_foreground() {
    _menu = new LcdMenu(this, lcd);
    _menu->set_labels(_menu_entries, callback(this, &AppLauncher::_menu_click));
    _menu->start();
}

void AppLauncher::on_background() {
    printf("MENU STOP OF LAUNCHER\n");
    _menu->stop();
    delete _menu;
}
