#include "launcher.h"
#include "kernel/kernel.h"
#include "app/blinker.h"

void AppLauncher::register_app(std::string name, AppBase *instance) {
    _app_list.push_back(std::pair<std::string, AppBase*>(name, instance));
}

void AppLauncher::run() {
    Kernel::get_instance().put_foreground(this, this);
    // TODO write a real luancher
    Kernel::get_instance().launch_app((*_app_list.begin()).second);
    Kernel::get_instance().put_foreground(this, (*_app_list.begin()).second);
}

void AppLauncher::release() {
    // TODO
}
