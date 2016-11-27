#include "app/base.h"
#include "kernel/kernel.h"


void AppBase::_launch() {
    // get a new event queue
    // TODO catch bad_alloc
    _m_thread = new Thread();
    _m_thread->start(callback(this, &AppBase::_on_launch));
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

bool AppBase::register_io_event_handler(IOEvent ev, AppVoidFuncPtr handler) {
    return Kernel::get_instance().register_io_event_handler(this, ev, handler);
}
