#ifndef WSMENUMANAGER_H
#define WSMENUMANAGER_H

#include "wsmenumanager_global.h"
#include "../WsCore/include/menumanager.h"
#include "../WsCore/include/window.h"

class WSMENUMANAGERSHARED_EXPORT WsMenuManager
        :public QObject, public Plugin::MenuManager
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGINMENUMANAGER)
    Q_INTERFACES(Plugin::MenuManager Plugin::Base)

public:
    WsMenuManager();
    virtual ~WsMenuManager() override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

    // MenuManager interface
public:
    virtual Plugin::MenuManager *newMenuManager(Core::Group::Visual *visual) override;

    virtual int groupCount() const override;
    virtual QMenu *menuGroup(int index) override;

    virtual void registerAction(QAction *action, Plugin::Base *host) override;
    virtual void menuActive(const QList<Plugin::Base *> &actived_set) override;
    virtual void removeRelatedMenus(Plugin::Base *plugin) override;


private:
    QList<QMenu*> menus;
    Core::Group::Visual *visual;
    //      源插件                    action   关联菜单
    QHash<Plugin::Base *, QList<QPair<QAction *, QMenu *> > *> actions_map;
    //      当前激活插件集合
    QList<Plugin::Base *> actived_stack;


    // 新建项目
    void files_fileNew(QAction *act){}
    void files_fileOpen(){}
    void files_fileSave(QAction *act){}
    void files_fileClose(QAction *act){}

    void files_projectNew(QAction *act);
    void files_projectOpen();
    void files_projectSave(QAction *act);
    void files_projectClose(QAction *act);

    // file-menus================
    QMenu *const file_new;
    QAction *const file_groups;
    QAction *const file_append;
    QMenu *const file_save;
    QMenu *const file_close;
    QMenu *const file_history;

    QMenu *const project_new;
    QAction *const project_open;
    QMenu *const project_save;
    QMenu *const project_close;
    QMenu *const project_history;

    QAction *const project_print;

    QAction *const exit;

    // help-menus====================


private:
    void refreshFilesMenu();

};

#endif // WSMENUMANAGER_H
