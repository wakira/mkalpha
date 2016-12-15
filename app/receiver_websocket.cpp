#include "kernel/kernel.h"
#include "oslib/io_devices.h"
#include "oslib/lcd.h"
#include "receiver_websocket.h"

AppReceiverWebSocket::AppReceiverWebSocket() : AppBase() {
    foreground = false;
    ethptr = new EthernetInterface();
};

void AppReceiverWebSocket::run() {
    ethptr->connect();

    url[] = "ws://10.42.0.1:4567/ws/client"; // TODO: remove hard-coding
    ws = new WebSocket(ethptr, url); // TODO: remove hard-coding

    printf("[AppReceiverWebSocket] connecting...\n");
    bool result = ws->connect();
    if (!result) {
        printf("[AppReceiverWebSocket] websocket connection failed\n");
        return;
    }

    printf("[AppReceiverWebSocket] start receiving\n");
    receive_loop();
}

void AppReceiverWebSocket::receive_loop() {
    while (true) {
        receive();
    }
}

void AppReceiverWebSocket::receive() {
    ws->read(msg);
    if (strlen(msg) > 0) {
        if (foreground) {
            // TODO: some output
        }
        printf("%s\n", msg);
    }
}

// temporary entrypoint
void AppReceiverWebSocket::start() {
    run();
}

void AppReceiverWebSocket::release() {
}

void AppReceiverWebSocket::on_foreground() {
    foreground = true;
}

void AppReceiverWebSocket::on_background() {
}
