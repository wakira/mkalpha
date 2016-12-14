#include "bubbles.h"
#include "kernel/kernel.h"
#include "oslib/io_devices.h"
#include "oslib/lcd.h"

AppBubbles::AppBubbles() : AppBase() {
    foreground = false;
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

    lcd_mutex.lock();
    width = (*lcd)->width();
    height = (*lcd)->height();
    lcd_mutex.unlock();

    Kernel& kernel = Kernel::get_instance();
    kernel.register_io_event_handler(this, JOYSTICK_FIRE,
            callback(this, &AppBubbles::on_joystick_fire));
    foreground = true;
}

void AppBubbles::on_background() {
    foreground = false;
    printf("BUBBLES ON_BG\n");

    Kernel& kernel = Kernel::get_instance();
    kernel.unregister_io_event_handler(this, JOYSTICK_FIRE);
}

void AppBubbles::proceed() {
    if (foreground) {
        lcd_mutex.lock();
        (*lcd)->cls();
        for (int ind = 0; ind < NUM_BUBBLES; ind++) {
            Bubble &b = bubbles[ind];

            b.x += b.dx;
            b.y += b.dy;
            bool draw = true;
            if (b.x - b.r < 0 || width <= b.x + b.r) { b.dx *= -1; draw = false;}
            if (b.y - b.r < 0 || height <= b.y + b.r) { b.dy *= -1; draw = false;}
            if (draw) { (*lcd)->circle(b.x, b.y, b.r, 1); };
        }
        (*lcd)->copy_to_lcd();
        lcd_mutex.unlock();
    }
}

void AppBubbles::on_joystick_fire() {
    go_background();
}
