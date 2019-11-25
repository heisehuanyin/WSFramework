#ifndef WSWINDOW_H
#define WSWINDOW_H

#include "wswindow_global.h"
#include "../WsCore/include/window.h"

#include <QListView>
#include <QMainWindow>
#include <QMenuBar>
#include <QTreeView>

class WSWINDOWSHARED_EXPORT WsWindow
        : public QObject, public Plugin::Window
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGINWINDOW)
    Q_INTERFACES(Plugin::Base Plugin::Window)

public:
    WsWindow();
    WsWindow(Core::Group::Visual *visual);
    virtual ~WsWindow() override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

    // Window interface
public:
    virtual Window* newWindow(Core::Group::Visual *visual) override;
    virtual void show() override;
    virtual void resize(int width, int height) override;
    virtual void loadMenuManager(Plugin::MenuManager *menus) override;
    virtual QWidget *getWidget() override;

private:
    Core::Group::Visual *const visual;
    QMainWindow *const window;
    QTreeView *const project_view;
    QTabWidget *const main;

    void showProjectManagerMenu(const QPoint &point);
    void newGroupNode();
    void newItemNode(QAction *item);
    void appendItemNode();
    void removeTargetFile();
    void removeReference();
};

#endif // WSWINDOW_H
