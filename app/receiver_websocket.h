#ifndef APP_RECEIVER_H_
#define APP_RECEIVER_H_

#include "base.h"
#include "oslib/io_events.h"
#include "mbed.h"
#include "rtos.h"

#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "WebSocket.h"

class AppReceiverWebSocket : public AppBase {
public:
    AppReceiverWebSocket();
    virtual void start();
protected:
    virtual void run();
    virtual void release();
    virtual void on_foreground();
    virtual void on_background();

private:
    void hello();

    void receive_loop();
    void receive();

    bool foreground;

    WebSocket* ws;
    EthernetInterface *ethptr;
    int receive_id;
    char msg[128];
};

#endif
