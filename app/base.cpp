#include "app/base.h"
#include "kernel/kernel.h"
#include "oslib/lcd.h"


void AppBase::_launch() {
    // get a new event queue
    // TODO catch bad_alloc
    _m_thread = new Thread();
    _m_thread->start(callback(this, &AppBase::_on_launch));
}

void AppBase::_fg() {
    // TODO request LCD
    Kernel &k = Kernel::get_instance();
    lcd_mutex.lock();
    lcd = (Lcd*)(k.request_device(this, DEVICE_LCD));
    lcd_mutex.unlock();
    _m_event_queue->call(callback(this, &AppBase::on_foreground));
}

void AppBase::_bg() {
    lcd_mutex.lock();
    delete lcd;
    lcd = 0;
    lcd_mutex.unlock();
    _m_event_queue->call(callback(this, &AppBase::on_background));
}

void AppBase::_on_launch() {
    // TODO catch bad_alloc
    _m_event_queue = new EventQueue(32 * EVENTS_EVENT_SIZE); // TODO no magic number
    _m_event_queue->call(callback(this, &AppBase::run));
    _m_event_queue->dispatch_forever();
}

void AppBase::_release() {
    // TODO
}

bool AppBase::register_io_event_handler(IOEvent ev, Callback<void()> handler) {
    return Kernel::get_instance().register_io_event_handler(this, ev, handler);
}

void AppBase::call(Callback<void()> cb) {
    _m_event_queue->call(cb);
}

int AppBase::call_in_ms(int ms, Callback<void()> cb) {
    return _m_event_queue->call_in(ms, cb);
    // TODO what does the return value mean?, see mbed source code
}

int AppBase::call_every_ms(int ms, Callback<void()> cb) {
    return _m_event_queue->call_every(ms, cb);
    // TODO what does the return value mean?, see mbed source code
}
