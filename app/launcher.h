#ifndef APP_LAUNCHER_H_
#define APP_LAUNCHER_H_

#include "base.h"

class AppLauncher : public AppBase {
public:
    void register_app(AppVoidFuncPtr launcher_ptr);
    virtual void run();
    // virtual void go_background();
    // virtual void go_foreground();
    // virtual void release();
};

#endif
