#include "bubbles.h"
#include "kernel/kernel.h"
#include "oslib/io_devices.h"
#include "oslib/lcd.h"

AppBubbles::AppBubbles() : AppBase() {
    foreground = false;
    lcd = (Lcd*)(Kernel::get_instance().request_device(this, DEVICE_LCD));
    width = (*lcd)->width();
    height = (*lcd)->height();
};

void AppBubbles::run() {
    for (int ind = 0; ind < NUM_BUBBLES; ind++) {
        int r = 3 + rand()%8;
        int x = rand()%(width - 2*r) + r; // x <- [r, width-1-r]
        int y = rand()%(height - 2*r) + r; // y <- [r, height-1-r]
        int dx = rand()%5 - 2, dy = rand()%5 - 2;

        bubbles[ind] = Bubble(x, y, dx, dy, r);
    }

    call_every_ms(1000 / BUBBLES_FPS, callback(this, &AppBubbles::proceed));
}

void AppBubbles::release() { }

void AppBubbles::on_foreground() {
    printf("BUBBLES ON_FG\n");
    foreground = true;

    Kernel& kernel = Kernel::get_instance();
    kernel.register_io_event_handler(this, JOYSTICK_FIRE,
            callback(this, &AppBubbles::on_joystick_fire));
} 

void AppBubbles::on_background() {
    foreground = false;

    Kernel& kernel = Kernel::get_instance();
    kernel.unregister_io_event_handler(this, JOYSTICK_FIRE);

    printf("BUBBLES ON_BG\n");
}

void AppBubbles::proceed() {
    if (foreground == true) {
        (*lcd)->cls();
        for (int ind = 0; ind < NUM_BUBBLES; ind++) {
            Bubble &b = bubbles[ind];
            (*lcd)->circle(b.x, b.y, b.r, 1);
        }
        (*lcd)->copy_to_lcd();
    }
    for (int ind = 0; ind < NUM_BUBBLES; ind++) {
        Bubble &b = bubbles[ind];

        b.x += b.dx;
        b.y += b.dy;
        if (b.x - b.r < 0 || width <= b.x + b.r) { b.dx *= -1; }
        if (b.y - b.r < 0 || height <= b.y + b.r) { b.dy *= -1; }
    }
}

void AppBubbles::on_joystick_fire() {
    go_background();
}
