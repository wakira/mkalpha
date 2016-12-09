#ifndef APP_LAUNCHER_H_
#define APP_LAUNCHER_H_

#include "base.h"
#include <list>
#include <utility>
#include <string>

class AppLauncher : public AppBase {
public:
    void register_app(std::string name, AppBase *instance);
    virtual void run();
    // virtual void go_background();
    // virtual void go_foreground();
    virtual void release();
private:
    std::list<std::pair<std::string, AppBase*> > _app_list;
};

#endif
