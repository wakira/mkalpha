#ifndef APP_BASE_H_
#define APP_BASE_H_

#include "oslib/io_events.h"
#include "mbed.h"
#include "rtos.h"
#include <string>


class Lcd;
class Kernel;

class AppBase {
    friend class Kernel;
public:
    AppBase() {
        _m_thread = 0;
        _m_event_queue = 0;
        _running = false;
    }
    void call(Callback<void()> cb);
    void call(Callback<void(std::string)> cb, std::string p1);
    int call_in_ms(int ms, Callback<void()> cb);
    int call_every_ms(int ms, Callback<void()> cb);
    bool is_running();
    Mutex lcd_mutex;
protected:
    virtual void run() = 0; // when app is first run
    virtual void on_background() {} // when app is put to background
    virtual void on_foreground() {} // when app goes back to foreground
    virtual void release() {} // when the kernel asks App to release all its resource locks
    bool register_io_event_handler(IOEvent ev, Callback<void()> handler);
    void go_background();
    Lcd *lcd;
private:
    bool _running;
    Thread *_m_thread;
    EventQueue *_m_event_queue;
    void _on_launch(); // executed in App's own thread, calls run()
    void _release(); // this function is called by kernel to preempt all acquired resources
    // functions called by kernel
    void _launch();
    void _fg();
    void _bg();
    void _bg_cont();
};

#endif
