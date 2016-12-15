#include "kernel/kernel.h"
#include "mbed.h"
#include "app/launcher.h"
#include "app/bubbles.h"
#include "app/blinker.h"
#include "app/blinker_2.h"
#include "app/receiver_websocket.h"

// Kernel &kernel = Kernel::get_instance();

int main() {
    printf("Hello\n");
    AppReceiverWebSocket *app = new AppReceiverWebSocket();
    Thread th;
    th.start(callback(app, &AppReceiverWebSocket::start));
    th.join();
    printf("Bye :(\n");
}
