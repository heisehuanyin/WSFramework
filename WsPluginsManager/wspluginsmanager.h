#ifndef WSPLUGINSMANAGER_H
#define WSPLUGINSMANAGER_H

#include "../WsCore/include/pluginsmanager.h"
#include "wspluginsmanager_global.h"



class WSPLUGINSMANAGERSHARED_EXPORT WsPluginsManager
        :public QObject, public Plugin::PluginsManager
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGINMANAGER)
    Q_INTERFACES(Plugin::PluginsManager Plugin::Base)

public:
    WsPluginsManager();
    virtual ~WsPluginsManager() override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;


    // PluginsManager interface
public:
    virtual void loadAllPlugins(const QString& plugins_home) override;
    virtual void unloadRelatedPlugins(Base* root) override;
    virtual QList<Plugin::Base *> queryRegisterdPlugins(Plugin::PluginType type) override;


    virtual Plugin::LogWriter *newLogWriter(Base *host, const QString &registerName, const QString &project_root_path, Plugin::Schedule *visual) override;
    virtual Plugin::ConfigPort *newConfigPort(Base *host, const QString &registerName, Plugin::Schedule *core) override;
    virtual Plugin::ProjectManager *newProjectManager(Plugin::Base *host, const QString &registerName, Core::Group::Visual *visual) override;

    virtual Plugin::MenuManager *newMenuManager(Base *host, const QString &regName, Core::Group::Visual *visual) override;
    virtual Plugin::Window* newWindow(Base *host, const QString &registerName, Core::Group::Visual *visual) override;

    virtual QList<Plugin::Base *> downstreamPlugins(Plugin::Base *base) override;
private:
    QMap<QString, Plugin::Base*> allPlugins;

    // 插件调用关系  插件               上游插件           下游插件
    QMap<Plugin::Base*, QPair<Plugin::Base*, QList<Plugin::Base*>*>> relates;

    //  构建插件关系          主调者     被调用者
    void buildRelation(Base *host, Base *next_ins);

};

#endif // WSPLUGINSMANAGER_H
