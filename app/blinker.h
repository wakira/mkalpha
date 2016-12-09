#ifndef APP_BLINKER_H_
#define APP_BLINKER_H_

#include "base.h"
#include "oslib/led.h"

class AppBlinker : public AppBase {
public:
    virtual void run();
    virtual void release();
private:
    Led *_led1;
    void _blink();
};

#endif
