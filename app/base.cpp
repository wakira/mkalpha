#include "app/base.h"
#include "kernel/kernel.h"
#include "oslib/lcd.h"


void AppBase::_launch() {
    // get a new event queue
    // TODO catch bad_alloc
    _m_thread = new Thread();
    _m_event_queue = new EventQueue(8 * EVENTS_EVENT_SIZE); // TODO no magic number
    _m_event_queue->call(callback(this, &AppBase::run));
    _m_thread->start(callback(this, &AppBase::_on_launch));
}

void AppBase::_fg() {
    Kernel &k = Kernel::get_instance();
    lcd = (Lcd*)(k.request_device(this, DEVICE_LCD));
    _m_event_queue->call(callback(this, &AppBase::on_foreground));
}

void AppBase::_bg() {
    on_background();
    _bg_cont();
    // _m_event_queue->call(callback(this, &AppBase::on_background));
    // _m_event_queue->call(callback(this, &AppBase::_bg_cont));
}

void AppBase::_bg_cont() {
    lcd_mutex.lock();
    delete lcd;
    lcd = 0;
    lcd_mutex.unlock();
}

void AppBase::_on_launch() {
    _running = true;
    _m_event_queue->dispatch_forever();
}


void AppBase::_release() {
    _m_event_queue->call(callback(this, &AppBase::on_release));
    _m_event_queue->call(callback(this, &AppBase::_terminate));
    _m_thread->join();
    delete _m_thread;
    delete _m_event_queue;
    _running = false;
    printf("after _release\n");
}

void AppBase::_terminate() {
    _m_thread->terminate();
}

bool AppBase::register_io_event_handler(IOEvent ev, Callback<void()> handler) {
    return Kernel::get_instance().register_io_event_handler(this, ev, handler);
}

void AppBase::call(Callback<void()> cb) {
    _m_event_queue->call(cb);
}

void AppBase::call(Callback<void(std::string)> cb, std::string p1) {
    _m_event_queue->call(cb, p1);
}

int AppBase::call_in_ms(int ms, Callback<void()> cb) {
    return _m_event_queue->call_in(ms, cb);
    // TODO what does the return value mean?, see mbed source code
}

int AppBase::call_every_ms(int ms, Callback<void()> cb) {
    return _m_event_queue->call_every(ms, cb);
    // TODO what does the return value mean?, see mbed source code
}

void AppBase::exit() {
    Kernel::get_instance().stop_app(this);
}

void AppBase::go_background() {
    Kernel::get_instance().put_background(this);
}

bool AppBase::is_running() {
    return _running;
}
