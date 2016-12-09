#include "kernel/kernel.h"
#include "app/launcher.h"
#include "app/blinker.h"

Kernel &kernel = Kernel::get_instance();

int main() {
    AppLauncher *launcher = new AppLauncher();
    // register apps to launcher
    launcher->register_app("Blinker", new AppBlinker());
    // register the launch function of launcher itself to kernel
    kernel.register_launcher(launcher);
    // run kernel
    kernel.run_kernel();
}
