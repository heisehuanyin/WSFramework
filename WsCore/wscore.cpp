#include "wscore.h"
#include "./include/pluginsmanager.h"
#include "./include/logwriter.h"
#include "./include/menumanager.h"
#include "./include/window.h"

#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QTime>
#include <QDebug>

using namespace Plugin;


WsCore::WsCore()
    :unique_plgsmgr(nullptr),global_logwriter(nullptr),global_configport(nullptr)
{
    auto software_home = QDir::homePath() + "/.WSFramework";
    plugins_dir_path = software_home + "/plugins";
    default_project_path = software_home + "/default";
    templates_path = software_home + "/template";


    auto config_home = QDir(software_home);
    if(!config_home.exists()){
        QDir::home().mkdir(".WSFramework");
    }
    if(!QDir(plugins_dir_path).exists()){
        config_home.mkdir("plugins");
    }
    if(!QDir(default_project_path).exists()){
        config_home.mkdir("default");
    }
    if(!QDir(templates_path).exists()){
        config_home.mkdir("template");

        QFile file(templates_path+"/PlainText.txt");
        if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
            QTextStream out(&file);
            out << "PlainText.txt";
            out.flush();
            file.flush();
            file.close();
        }
    }

    if(!(this->unique_plgsmgr = loadManager())){
        QMessageBox::critical(nullptr, "程序运行错误", "无法获取PluginManager插件！");
        exit(-1);
    }

    // ==variables init====
    this->unique_plgsmgr->loadAllPlugins(plugins_dir_path);

    this->global_logwriter = unique_plgsmgr->newLogWriter(this, "WsLogWriter", software_home, this);
    this->global_configport = unique_plgsmgr->newConfigPort(this, "WsConfigPort", this);
    if(config_home.exists("global.config")){
        this->global_configport->openFile(software_home+"/global.config");
    }
    else {
        this->global_configport->newFile(software_home+"/global.config");
    }
}

WsCore::~WsCore()
{
    if(unique_plgsmgr){
        // 一切起源于WsCore，也从WsCore销毁
        unique_plgsmgr->unloadRelatedPlugins(this);
        delete unique_plgsmgr;
        unique_plgsmgr = nullptr;
    }
}

void WsCore::openDefaultProject(Core::Group::Visual *vgroup)
{
    QDir default_pjt(getDefaultProjectDir());
    auto x = default_pjt.entryInfoList(QStringList()<<"*.project", QDir::Files);
    if(x.size() > 0)
        vgroup->openProject(x.at(0).absoluteFilePath());
    else
        vgroup->newProject("WsProjectManager", getDefaultProjectDir());

    vgroup->refreshVisibleMenus();
}

Core::Group::Visual *WsCore::openSilent()
{
    auto vg = new VGroup(this, unique_plgsmgr);
    visual_sets.append(vg);

    return vg;
}

Core::Group::Visual *WsCore::openWindow()
{
    auto vg = openSilent();
    static_cast<VGroup*>(vg)->openWindow();
    // TODO refresh menus

    return vg;
}

void WsCore::writeLog(const Base *base, const QString &title, const QString &message)
{
    writeLogPrimary("log", base->registerName(), title, message);
}

void WsCore::writeError(const Base *base, const QString &title, const QString &message)
{
    writeLogPrimary("error", base->registerName(), title, message);
    QMessageBox::critical(nullptr, title, base->registerName()+"::"+message);
}

void WsCore::writeWarning(const Base *base, const QString &title, const QString &message)
{
    writeLogPrimary("warning",base->registerName(), title, message);
    QMessageBox::warning(nullptr, title, base->registerName()+"::"+message);
}

void WsCore::test()
{
    QFileInfo f(".");
    qDebug() << f.absoluteFilePath();
}

ConfigPort *WsCore::globalConfigPort() const
{
    return this->global_configport;
}

QList<QString> WsCore::allProjectsType() const
{
    QList<QString> all;

    auto list = unique_plgsmgr->queryRegisterdPlugins(PluginType::PROJECTMANAGER);
    for (auto item : list) {
        all.append(item->registerName());
    }

    return all;
}

QString WsCore::getTemplatesPath() const
{
    return templates_path;
}

QString WsCore::registerName() const
{
    return "WsCore";
}

PluginType WsCore::pluginType() const
{
    return Plugin::LUNCH_SCHEDULE;
}

void WsCore::save()
{

}



QString WsCore::getPluginsDir() const
{
    return plugins_dir_path;
}

QString WsCore::getDefaultProjectDir() const
{
    return this->default_project_path;
}

PluginsManager *WsCore::loadManager()
{
    QDir plugins_home = QDir(getPluginsDir());

    auto plugins_list = plugins_home.entryInfoList();
    for (auto plugin : plugins_list) {
        if(!plugin.isFile())
            continue;

        QPluginLoader loader(plugin.absoluteFilePath());
        auto ins = loader.instance();
        if(!ins)
            continue;

        // load success
        auto pins = qobject_cast<Plugin::Base*>(ins);
        if(pins->pluginType() != Plugin::PLUGINSMANAGER){
            delete pins;
            loader.unload();
            continue;
        }

        return static_cast<Plugin::PluginsManager*>(pins);
    }
    return nullptr;
}

void WsCore::writeLogPrimary(const QString &type, const QString &plgName, const QString &title, const QString &message)
{
    QMutexLocker locker(&log_mutex);

    QDateTime time_stamp = QDateTime::currentDateTime();
    this->logstack.append(std::make_tuple(time_stamp.toMSecsSinceEpoch(),
                                          type, plgName, title, message));

    if(global_logwriter){
        for (auto item : logstack) {
            auto time = std::get<0>(item);
            auto type = std::get<1>(item);
            auto plgName = std::get<2>(item);
            auto title = std::get<3>(item);
            auto message = std::get<4>(item);

            if (type == "log") {
                global_logwriter->writeLog(time, title, message);
            }
            else if (type == "error") {
                global_logwriter->writeError(time, title, message);
            }
            else {
                global_logwriter->writeWarning(time, title, message);
            }
        }
        logstack.clear();
    }
}


