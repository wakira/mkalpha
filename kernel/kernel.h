#ifndef KERNEL_H_
#define KERNEL_H_

#include "app/base.h"
#include "oslib/io_events.h"
#include "mbed.h"
#include "oslib/io_events.h"
#include <vector>
#include <map>
#include <string>

// defines the singleton kernel
// we define no destructor since the kernel should run forever

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

    void register_launcher(AppBase *instance, AppVoidFuncPtr launcher_ptr);
    bool register_io_event_handler(AppBase *app, IOEvent event, AppVoidFuncPtr handler);
    void launch_app(AppBase *instance, AppVoidFuncPtr launcher_ptr);
    void run_kernel(); // run_kernel() never returns
private:
    // private routines
    void _panic(std::string info = "");
    void _launch_app(AppBase *instance, AppVoidFuncPtr launcher_ptr);
    void _setup_isr();
    void _on_io_event(IOEvent ev); 

    // isr
    void _isr_joystick_fire_rise();
    void _isr_joystick_fire_fall();
    // TODO add all joystick button handlers
private:
    // private states
    Mutex _kernel_mutex;
    EventQueue *_event_queue;
    bool _running;
    AppBase *_launcher_instance;
    AppVoidFuncPtr _launcher_launch;
    std::vector<AppBase*> _apps_running;
    std::map<AppBase*, std::map<IOEvent, AppVoidFuncPtr> > _app_io_handlers;
    AppBase *_app_foreground;

    // IO devices
    InterruptIn *_joystick_fire;
};

#endif
