#ifndef WSCORE_H
#define WSCORE_H

#include "./include/schedule.h"
#include "groups.h"

/**
 * @brief The CoreLuncher class
 * 用于载入插件管理器，开启[静默·图形]模式
 */
class WsCore : public Plugin::Schedule
{
public:
    WsCore();
    virtual ~WsCore() override;

    void openDefaultProject(Core::Group::Visual *vgroup);

    virtual Core::Group::Visual *openSilent() override;
    virtual Core::Group::Visual *openWindow() override;

    virtual void writeLog(const Base* base, const QString &title, const QString &message) override;
    virtual void writeError(const Base* base, const QString &title, const QString &message) override;
    virtual void writeWarning(const Base* base, const QString &title, const QString &message) override;

    virtual void test();

    virtual Plugin::ConfigPort *globalConfigPort() const override;
    virtual QList<QString> allProjectsType() const override;
    QString getTemplates_path() const override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

private:
    QString plugins_dir_path;
    QString default_project_path;
    QString templates_path;

    Plugin::PluginsManager *unique_plgsmgr;
    Plugin::LogWriter *global_logwriter;
    Plugin::ConfigPort *global_configport;

    QList<VGroup*> visual_sets;

    QMutex log_mutex;
    QList<std::tuple<qint64, QString, QString,  QString, QString>> logstack;

    virtual QString getPluginsDir() const;
    virtual QString getDefaultProjectDir() const;

    Plugin::PluginsManager *loadManager();
    void writeLogPrimary(const QString& type,const QString &plgName,const QString &title, const QString& message);
};


#endif // WSCORE_H
