#ifndef APP_BLINKER_H_
#define APP_BLINKER_H_

#include "base.h"

class AppBlinker : public AppBase {
public:
    virtual void run();
    virtual void release();
};

#endif
