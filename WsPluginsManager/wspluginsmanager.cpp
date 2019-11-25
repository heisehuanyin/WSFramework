#include "wspluginsmanager.h"
#include "../WsCore/include/logwriter.h"

#include <QPluginLoader>
#include <QMenu>
#include <QFileInfo>
#include <QObject>
#include <QDir>

WsPluginsManager::WsPluginsManager()
{

}

WsPluginsManager::~WsPluginsManager()
{
    auto keys = relates.keys();
    for (auto key : keys) {
        if(relates.contains(key)){
            unloadRelatedPlugins(key);
        }
    }

    for(auto plg : allPlugins){
        delete plg;
    }
}


QString WsPluginsManager::registerName() const
{
    return "WsPluginsManager";
}

Plugin::PluginType WsPluginsManager::pluginType() const
{
    return Plugin::PLUGINSMANAGER;
}

void WsPluginsManager::save()
{

}



void WsPluginsManager::loadAllPlugins(const QString &plugins_home)
{
    QDir plgHome(plugins_home);
    auto allentry = plgHome.entryInfoList();

    // 迭代加载每个插件
    for(auto file : allentry){
        if(!file.isFile())
            continue;

        QPluginLoader loader(file.absoluteFilePath());
        if(loader.isLoaded())
            continue;

        auto ibase = loader.instance();
        if(!ibase) continue;

        auto ins = qobject_cast<Plugin::Base*>(ibase);

        if(allPlugins.contains(ins->registerName()))
            delete allPlugins.value(ins->registerName());

        allPlugins.insert(ins->registerName(), ins);
    }
}

void WsPluginsManager::unloadRelatedPlugins(Plugin::Base *root)
{
    if (!relates.contains(root))
        return;

    auto record = relates.value(root);
    if(!record.first) return;

    if (record.second->size() == 0) {
        auto super = relates.value(record.first);
        super.second->removeAll(root);

        delete record.second;
        relates.remove(root);
        delete root;
    }
    else {
        for(auto item : *record.second)
            unloadRelatedPlugins(item);
    }
}

Plugin::LogWriter *WsPluginsManager::newLogWriter(Plugin::Base *host, const QString &registerName,
                                                  const QString &project_root_path,
                                                  Plugin::Schedule *visual)
{
    if(!allPlugins.contains(registerName)){
        visual->writeError(this, "运行错误", QString("找不到指定插件%1").arg(registerName));
        return nullptr;
    }

    auto target = allPlugins.value(registerName);
    auto plugin = static_cast<Plugin::LogWriter*>(target);

    plugin = plugin->newLogWriter(visual, project_root_path);
    buildRelation(host, plugin);

    return plugin;
}

Plugin::ConfigPort *WsPluginsManager::newConfigPort(Plugin::Base *host,
                   const QString &registerName, Plugin::Schedule *core)
{
    if(!allPlugins.contains(registerName)){
        core->writeError(this, "运行错误", QString("找不到指定插件%1").arg(registerName));
        return nullptr;
    }

    auto target = allPlugins.value(registerName);
    auto plugin = static_cast<Plugin::ConfigPort*>(target);

    plugin = plugin->newConfigPort(core);
    buildRelation(host, plugin);

    return plugin;
}

Plugin::Window *WsPluginsManager::newWindow(Plugin::Base *host, const QString &registerName, Core::Group::Visual *visual)
{
    if(!allPlugins.contains(registerName)){
        visual->schedule()->writeError(this, "运行错误", QString("找不到指定插件%1").arg(registerName));
        return nullptr;
    }

    auto target = allPlugins.value(registerName);
    auto plugin = static_cast<Plugin::Window*>(target);

    plugin = plugin->newWindow(visual);
    buildRelation(host, plugin);

    return plugin;
}

QList<Plugin::Base *> WsPluginsManager::downstreamPlugins(Plugin::Base *base)
{
    QList<Base*> vlist;

    auto a_relate = relates.value(base);
    for (auto dplugin : *a_relate.second) {
        vlist.append(dplugin);
        vlist.append(downstreamPlugins(dplugin));
    }

    return vlist;
}

Plugin::ProjectManager *WsPluginsManager::newProjectManager(Plugin::Base *host, const QString &registerName, Core::Group::Visual *visual)
{
    if(!allPlugins.contains(registerName)){
        visual->schedule()->writeError(this, "运行错误", QString("找不到指定插件%1").arg(registerName));
        return nullptr;
    }

    auto target = allPlugins.value(registerName);
    auto plugin = static_cast<Plugin::ProjectManager*>(target);

    plugin = plugin->newManager(visual);
    buildRelation(host, plugin);

    return plugin;
}

Plugin::MenuManager *WsPluginsManager::newMenuManager(Plugin::Base *host, const QString &regName, Core::Group::Visual *visual)
{
    if(!allPlugins.contains(regName)){
        visual->schedule()->writeError(this, "运行错误", QString("找不到制定插件%1").arg(regName));
        return nullptr;
    }

    auto target = allPlugins.value(regName);
    auto plugin = static_cast<Plugin::MenuManager*>(target);

    plugin = plugin->newMenuManager(visual);
    buildRelation(host, plugin);

    return plugin;
}

QList<Plugin::Base *> WsPluginsManager::queryRegisterdPlugins(Plugin::PluginType type)
{
    QList<Plugin::Base*> list;

    for (auto item : allPlugins) {
        if(item->pluginType() == type)
            list.append(item);
    }

    return list;
}

void WsPluginsManager::buildRelation(Plugin::Base *host, Plugin::Base *next_ins)
{
    // host # WsCore
    if(!relates.contains(host)){
        auto down=new QList<Base*>();
        relates.insert(host, qMakePair(nullptr, down));
    }

    auto pair = relates.value(host);
    pair.second->append(next_ins);

    if(!relates.contains(next_ins)){
        auto down=new QList<Base*>();
        relates.insert(next_ins, qMakePair(host, down));
    }
}









