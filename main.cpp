#include "kernel/kernel.h"
#include "app/launcher.h"
#include "app/blinker.h"
#include "app/blinker_2.h"

Kernel &kernel = Kernel::get_instance();

int main() {
    AppLauncher *launcher = new AppLauncher();
    // register apps to launcher
    launcher->register_app("Blinker1", new AppBlinker());
    launcher->register_app("Blinker2", new AppBlinker2());
    // register the launch function of launcher itself to kernel
    kernel.register_launcher(launcher);
    // run kernel
    kernel.run_kernel();
}
