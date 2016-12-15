#ifndef APP_BUBBLES_H_
#define APP_BUBBLES_H_

#include "base.h"
#include "oslib/io_events.h"
#include "mbed.h"

struct Bubble {
    int x, y, dx, dy, r;
    Bubble() {}
    ~Bubble() {}
    Bubble(int x_, int y_, int dx_, int dy_, int r_): x(x_), y(y_), dx(dx_), dy(dy_), r(r_) {}
};

class AppBubbles : public AppBase {
public:
    AppBubbles();
protected:
    virtual void run();
    virtual void on_release();
    virtual void on_foreground();
    virtual void on_background();
private:
    virtual void proceed();

    const static int BUBBLES_FPS = 25;
    const static int NUM_BUBBLES = 10;
    Bubble bubbles[NUM_BUBBLES];
    int width;
    int height;

    bool foreground;
};

#endif
