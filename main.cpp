#include "kernel/kernel.h"
#include "app/launcher.h"
#include "app/blinker.h"
#include "app/bubbles.h"

#include "mbed.h"

Kernel &kernel = Kernel::get_instance();


int main() {
    // a rough way to set random seed, you can replace this with other algorithm
    AnalogIn* seed = new AnalogIn(p16);
    srand((int)(seed->read() * 0x7fffffff));
    delete seed;

    AppLauncher *launcher = new AppLauncher();
    // register apps to launcher
    launcher->register_app("Blinker", new AppBlinker());
    launcher->register_app("Bubbles", new AppBubbles());
    // register the launch function of launcher itself to kernel
    kernel.register_launcher(launcher);
    // run kernel
    kernel.run_kernel();
}
