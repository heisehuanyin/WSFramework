#ifndef CONFIGPORT_H
#define CONFIGPORT_H

#include "schedule.h"

namespace Plugin {
    class ConfigPort : public Base
    {
    public:
        virtual ~ConfigPort() = default;

        virtual ConfigPort *newConfigPort(Plugin::Schedule *pjt) = 0;
        virtual QString filePath() const = 0;

        virtual void openFile(const QString &file_path) = 0;
        virtual void newFile(const QString &project_root_path) = 0;

        virtual void setValue(const Base *base, const QString &key, const QString &val) = 0;
        virtual QString getValue(const Base *base, const QString &key) const = 0;

        virtual void setList(const Base *base, const QString &name, const QList<QString> &list) = 0;
        virtual QList<QString> getList(const Base *base, const QString &name) const = 0;

        virtual void setMap(const Base *base, const QString &name, const QHash<QString, QString> &map) = 0;
        virtual QHash<QString, QString> getMap(const Base *base, const QString &name) const = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGIN_CONFIGPORT "wsframework.plugin-set.configport"
Q_DECLARE_INTERFACE(Plugin::ConfigPort, PLUGIN_CONFIGPORT)

QT_END_NAMESPACE

#endif // CONFIGPORT_H
