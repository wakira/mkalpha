#ifndef APP_LAUNCHER_H_
#define APP_LAUNCHER_H_

#include "base.h"
#include <map>
#include <list>
#include <string>

class LcdMenu;

class AppLauncher : public AppBase {
public:
    void register_app(std::string name, AppBase *instance);
protected:
    virtual void run();
    virtual void on_background();
    virtual void on_foreground();
    virtual void release();
private:
    void _menu_click(std::string label);
    std::map<std::string, AppBase*> _app_list;
    std::list<std::string> _menu_entries;
    LcdMenu *_menu;
};

#endif
