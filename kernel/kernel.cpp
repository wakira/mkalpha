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
    _lcd_factory = new LcdFactory();
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
    printf("RUN KERNEL\n");
    _running = true;
    _kernel_mutex.unlock();
    _event_queue = new EventQueue(12 * EVENTS_EVENT_SIZE); // TODO magic number
    osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
    // set up interrupt handlers
    _setup_isr();
    C12832 lcd(p5, p7, p6, p8,p11);
    lcd.cls();
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

void Kernel::stop_app(AppBase *instance) {
    _event_queue->call(callback(this, &Kernel::_stop_app),
            instance);
}

void Kernel::_launch_app(AppBase *instance) {
    printf("launch new app\n");
    _kernel_mutex.lock(); // TODO it seems that there is no need for locks
    // _apps_running.push_back(instance);
    instance->_launch();
    _kernel_mutex.unlock();
}

void Kernel::_stop_app(AppBase *instance) {
    printf("stop app\n");
    if (_app_foreground == _launcher_instance) {
        printf("do not kill launcher\n");
        return;
    }
    if (_app_foreground == instance) {
        _put_background(instance);
    }
    instance->_release();
    for(std::list<Device*>::iterator it = _app_devices[instance].begin();
            it != _app_devices[instance].end(); it++) {
        IODevice id = _allocated_devices_rev[*it];
        _allocated_devices_rev.erase(*it);
        _allocated_devices.erase(id);
        delete *it;
    }
    _app_devices[instance].clear();
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
            _put_background(_app_foreground);
            break;
        case JOYSTICK_LONGER_PRESS:
            _stop_app(_app_foreground);
            break;
        // and other events...
        default:
            _panic("invalid io event");
    }
}

void Kernel::_isr_joystick_fire_rise() {
    _fire_timer.start();
}

void Kernel::_isr_joystick_fire_fall() {
    _fire_timer.stop();
    // TODO another magic number
    if (_fire_timer.read_ms() > 1500) {
        _event_queue->call(callback(this, &Kernel::_on_io_event),
                JOYSTICK_LONGER_PRESS);
    }
    else if (_fire_timer.read_ms() > 300) {
        _event_queue->call(callback(this, &Kernel::_on_io_event),
                JOYSTICK_LONG_PRESS);
    } else {
        _event_queue->call(callback(this, &Kernel::_on_io_event),
                JOYSTICK_FIRE);
    }
    _fire_timer.reset();
}

void Kernel::_isr_joystick_up_rise() {
    _event_queue->call(callback(this, &Kernel::_on_io_event), JOYSTICK_UP);
}

void Kernel::_isr_joystick_down_rise() {
    _event_queue->call(callback(this, &Kernel::_on_io_event), JOYSTICK_DOWN);
}

void Kernel::_isr_joystick_left_rise() {
    _event_queue->call(callback(this, &Kernel::_on_io_event), JOYSTICK_LEFT);
}

void Kernel::_isr_joystick_right_rise() {
    _event_queue->call(callback(this, &Kernel::_on_io_event), JOYSTICK_RIGHT);
}

void Kernel::_setup_isr() {
    _joystick_fire = new InterruptIn(p14);
    _joystick_up = new InterruptIn(p15);
    _joystick_down = new InterruptIn(p12);
    _joystick_left = new InterruptIn(p13);
    _joystick_right = new InterruptIn(p16);
    _joystick_fire->rise(callback(this, &Kernel::_isr_joystick_fire_rise));
    _joystick_fire->fall(callback(this, &Kernel::_isr_joystick_fire_fall));
    _joystick_up->rise(callback(this, &Kernel::_isr_joystick_up_rise));
    _joystick_down->rise(callback(this, &Kernel::_isr_joystick_down_rise));
    _joystick_left->rise(callback(this, &Kernel::_isr_joystick_left_rise));
    _joystick_right->rise(callback(this, &Kernel::_isr_joystick_right_rise));
    // TODO other devices
}

bool Kernel::register_io_event_handler(AppBase *app, IOEvent event,
        Callback<void()> handler) {
    if (event >= JOYSTICK_LONG_PRESS) { // event is not registrable
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
        case DEVICE_LED4: {
            Device *device = new LedDevice(id);
            _allocated_devices[id] = device;
            _allocated_devices_rev[device] = id;
            if (_app_devices.count(app) == 0) {
                _app_devices[app] = std::list<Device*>();
            }
            _app_devices[app].push_back(device);
            return device;
        }
        case DEVICE_LCD:
            return _lcd_factory->request_lcd_device(app);
        default:
            return 0; // invalid device id
    }
}

void Kernel::put_foreground(AppBase *launcher, AppBase *target) {
    if (launcher != _launcher_instance) {
        printf("invliad fg\n");
        return;
    }
    _event_queue->call(callback(this, &Kernel::_put_foreground), target);
}

void Kernel::_put_foreground(AppBase *target) {
    // put to background the current fg
    if (_app_foreground) {
        printf("_fg->bg()\n");
        _app_foreground->_bg();
    }
    // put to foreground target
    _app_foreground = target;
    printf("target->fg()\n");
    target->_fg();
}

void Kernel::put_background(AppBase *target) {
    if (target != _app_foreground) {
        printf("invalid bg\n");
        return;
    }
    _event_queue->call(callback(this, &Kernel::_put_background), target);
}

void Kernel::_put_background(AppBase *target) {
    if (target == _launcher_instance) {
        printf("do not put launcher to bg\n");
        return;
    }
    _app_foreground->_bg();
    _app_foreground = _launcher_instance;
    _app_foreground->_fg();
}
