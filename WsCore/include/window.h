#ifndef WINDOW_H
#define WINDOW_H

#include "schedule.h"
#include "menumanager.h"

namespace Plugin {
    class Window : public Base
    {
    public:
        virtual ~Window() = default;

        virtual Window *newWindow(Core::Group::Visual *core) = 0;
        virtual QWidget *getWidget() = 0;

        virtual void show() = 0;
        virtual void resize(int width = 400, int height = 300) = 0;

        virtual void loadMenuManager(MenuManager* menus) = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGINWINDOW "wsframework.plugin-set.window"
Q_DECLARE_INTERFACE(Plugin::Window, PLUGINWINDOW)

QT_END_NAMESPACE

#endif // WINDOW_H
