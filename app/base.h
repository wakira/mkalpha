#ifndef APP_BASE_H_
#define APP_BASE_H_

#include "mbed.h"
#include "rtos.h"
#include "oslib/io_events.h"

class AppBase;

// AppVoidFuncPtr is the function pointer to void function in in AppBase
typedef void (AppBase::*AppVoidFuncPtr)();

class AppBase {
public:
    AppBase() {
        _m_thread = 0;
        _m_event_queue = 0;
    }
    virtual void run() = 0; // when app is first run
    virtual void go_background() {} // when app is put to background
    virtual void go_foreground() {} // when app goes back to foreground
    virtual void release(); // when the kernel asks App to release all its resource locks
    void _launch(); // this function is called by launcher
protected:
    bool register_io_event_handler(IOEvent ev, AppVoidFuncPtr handler);
private:
    Thread *_m_thread;
    EventQueue *_m_event_queue;
    void _on_launch(); // executed in App's own thread, calls run()
    void _release(); // this function is called by kernel to preempt all acquired resources
};


#endif
