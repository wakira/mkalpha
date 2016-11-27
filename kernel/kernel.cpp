#include "kernel.h"
#include "rtos.h"
#include "C12832.h"

Kernel::Kernel() {
    _running = false;
    _app_id_cnt = 0;
    _launcher_instance = 0;
    _launcher_launch = 0;
    _app_foreground = 0;
    _joystick_fire = 0;
}

void Kernel::_panic(std::string info) {
    C12832 lcd(p5, p7, p6, p8,p11);
    lcd.cls();
    lcd.locate(0, 3);
    lcd.printf("KERNEL PANIC");
    lcd.locate(0, 15);
    lcd.printf(info.c_str());
    while (1) {
        // infinite loop of death
    }
}

void Kernel::register_launcher(AppBase *instance, AppVoidFuncPtr launcher_ptr) {
    _kernel_mutex.lock();
    if (_running) {
        _panic("register_launcher: already running");
    }
    _launcher_instance = instance;
    _launcher_launch = launcher_ptr;
    _kernel_mutex.unlock();
}

// Kernel::run_kernel() should never return
void Kernel::run_kernel() {
    _kernel_mutex.lock();
    if (_running) {
        // indicate kernel panic
        _panic();
    }
    _running = true;
    _kernel_mutex.unlock();
    _event_queue = new EventQueue(32 * EVENTS_EVENT_SIZE); // TODO magic number
    osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
    // set up interrupt handlers
    _setup_isr();
    // launch the first App (Launcher)
    _event_queue->call(callback(this, &Kernel::_launch_app)
            ,_launcher_instance, _launcher_launch);
    // execute Event Loop
    _event_queue->dispatch_forever();
}

void Kernel::launch_app(int *instance, int launch_ptr) {
    _event_queue->call(callback(this, &Kernel::_launch_app),
            instance, launch_ptr);
}

void Kernel::_launch_app(AppBase *instance, AppVoidFuncPtr launcher_ptr) {
    _kernel_mutex.lock();
    _id_apps_running[instance] = _app_id_cnt;
    _app_id_cnt += 1;
    _app_foreground = instance;
    (instance->*launcher_ptr)();
    _kernel_mutex.unlock();
}

void Kernel::_on_io_event(IOEvent ev) {
    switch (ev) {
        switch JOYSTICK_FIRE:
        switch JOYSTICK_UP:
        switch JOYSTICK_DOWN:
        switch JOYSTICK_LEFT:
        switch JOYSTICK_RIGHT:
            // TODO call foreground app's registered handler
            break;
        switch JOYSTICK_LONG_PRESS:
            // TODO kill the foreground app, reclaim resources held by that app
            break;
        // and other events...
        default:
            _panic("invalid io event");
    }
}

void Kernel::_isr_joystick_fire_rise() {
    // TODO determine if it is long press
    _event_queue->call(callback(this, &Kernel::_on_io_event), JOYSTICK_FIRE);
}

void Kernel::_isr_joystick_fire_fall() {
    // TODO determine if it is long press
}

void Kernel::_setup_isr() {
    _joystick_fire = new InterruptIn(p14);
    _joystick_fire->rise(callback(this, &Kernel::_isr_joystick_fire_rise));
    _joystick_fire->fall(callback(this, &Kernel::_isr_joystick_fire_fall));
    // TODO
}

void Kernel::register_io_event_handler(AppBase *app, IOEvent event,
        AppVoidFuncPtr handler) {
    // TODO
}
