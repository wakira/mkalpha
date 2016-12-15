#ifndef KERNEL_H_
#define KERNEL_H_

#include "app/base.h"
#include "oslib/io_events.h"
#include "oslib/io_devices.h"
#include "mbed.h"
#include <list>
#include <map>
#include <string>

// defines the singleton kernel
// we define no destructor since the kernel should run forever

class LcdFactory;

class Kernel {
private:
    Kernel();

    // no copy is allowed
    Kernel(Kernel const&);
    void operator=(Kernel const&);
public:
    static Kernel& get_instance() {
        static Kernel KERNEL_INSTANCE;
        return KERNEL_INSTANCE;
    }

    // syncrhonous calls
    void register_launcher(AppBase *instance);
    bool register_io_event_handler(AppBase *app, IOEvent event, Callback<void()> handler);
    void unregister_io_event_handler(AppBase *app, IOEvent event);
    Device* request_device(AppBase *app, IODevice id);
    // asynchronous calls
    void put_foreground(AppBase *launcher, AppBase *target);
    void put_background(AppBase *target);
    void launch_app(AppBase *instance);
    void stop_app(AppBase *instance);

    // start the kernel
    void run_kernel(); // run_kernel() never returns
private:
    // private routines
    void _panic(std::string info = "");
    void _launch_app(AppBase *instance);
    void _stop_app(AppBase *instance);
    void _setup_isr();
    void _on_io_event(IOEvent ev);
    void _put_foreground(AppBase *target);
    void _put_background(AppBase *target);

    // isr
    void _isr_joystick_fire_rise();
    void _isr_joystick_fire_fall();
    void _isr_joystick_up_rise();
    void _isr_joystick_down_rise();
    void _isr_joystick_left_rise();
    void _isr_joystick_right_rise();
    // TODO add all joystick button handlers
private:
    // private states
    Mutex _kernel_mutex;
    EventQueue *_event_queue;
    bool _running;
    AppBase *_launcher_instance;
    // std::list<AppBase*> _apps_running;
    std::map<AppBase*, std::map<IOEvent, Callback<void()> > > _app_io_handlers;
    AppBase *_app_foreground;
    LcdFactory *_lcd_factory;

    // IO devices
    InterruptIn *_joystick_fire;
    Timer _fire_timer;
    InterruptIn *_joystick_up;
    InterruptIn *_joystick_down;
    InterruptIn *_joystick_left;
    InterruptIn *_joystick_right;
    std::map<IODevice, Device*> _allocated_devices;
    std::map<Device*, IODevice> _allocated_devices_rev;
    std::map<AppBase*, std::list<Device*> > _app_devices;
};

#endif
