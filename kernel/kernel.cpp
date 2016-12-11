#include "kernel/device/led.h"
#include "kernel/device/lcd.h"
#include "kernel.h"
#include "rtos.h"
#include "C12832.h"

Kernel::Kernel() {
    _running = false;
    _launcher_instance = 0;
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

void Kernel::register_launcher(AppBase *instance) {
    _kernel_mutex.lock();
    if (_running) {
        _panic("register_launcher: already running");
    }
    _launcher_instance = instance;
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
            ,_launcher_instance);
    // execute Event Loop
    _event_queue->dispatch_forever();
}

void Kernel::launch_app(AppBase *instance) {
    _event_queue->call(callback(this, &Kernel::_launch_app),
            instance);
}

void Kernel::_launch_app(AppBase *instance) {
    _kernel_mutex.lock();
    _apps_running.push_back(instance);
    instance->_launch();
    _kernel_mutex.unlock();
}

void Kernel::_on_io_event(IOEvent ev) {
    switch (ev) {
        case JOYSTICK_FIRE:
        case JOYSTICK_UP:
        case JOYSTICK_DOWN:
        case JOYSTICK_LEFT:
        case JOYSTICK_RIGHT: {
            if (!_app_foreground) {
                return;
            }
            // call foreground app's registered handler
            std::map<IOEvent, Callback<void()> > &ev_cb_map = _app_io_handlers[_app_foreground];
            if (ev_cb_map.count(ev) != 0) {
                Callback<void()> cb = ev_cb_map[ev];
                _app_foreground->_m_event_queue->call(cb);
            }
            break;
        }
        case JOYSTICK_LONG_PRESS:
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

bool Kernel::register_io_event_handler(AppBase *app, IOEvent event,
        Callback<void()> handler) {
    if (event <= JOYSTICK_LONG_PRESS) { // event is not registrable
        return false;
    }
    // TODO do some checking
    if (_app_io_handlers.count(app) == 0) {
        _app_io_handlers[app] = std::map<IOEvent, Callback<void()> >();
    }
    _app_io_handlers[app][event] = handler;
    return true;
}

void Kernel::unregister_io_event_handler(AppBase *app, IOEvent event) {
    std::map<IOEvent, Callback<void()> > &ev_cb_map = _app_io_handlers[app];
    ev_cb_map.erase(event);
}

Device* Kernel::request_device(AppBase *app, IODevice id) {
    if (_allocated_devices.count(id) != 0) { // already allocated
        return 0;
    }
    switch (id) {
        case DEVICE_LED1:
        case DEVICE_LED2:
        case DEVICE_LED3:
        case DEVICE_LED4:
            _allocated_devices[id] = app;
            return new LedDevice(id);
        case DEVICE_LCD:
            // TODO check if running in kernel thread
            return new LcdDevice(app);
        default:
            return 0; // invalid device id
    }
}

void Kernel::put_foreground(AppBase *launcher, AppBase *target) {
    if (launcher != _launcher_instance) {
        return;
    }
    _event_queue->call(callback(this, &Kernel::_put_foreground), target);
}

void Kernel::_put_foreground(AppBase *target) {
    // put to background the current fg
    if (_app_foreground) {
        _app_foreground->_bg();
    }
    // put to foreground target
    _app_foreground = target;
    target->_fg();
}
