#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "foundation.h"
#include "datatype.h"

namespace Plugin {
    class PluginsManager;
    class LogWriter;
    class ProjectManager;
    class MenuManager;
    class Window;
    class ConfigPort;
}

namespace Plugin {
    class Schedule : public Base
    {
    public:
        virtual ~Schedule() override = default;

        virtual Core::Group::Visual *openSilent() = 0;
        virtual Core::Group::Visual *openWindow() = 0;

        virtual void writeLog(const Base* base, const QString &title, const QString &message) = 0;
        virtual void writeError(const Base* base, const QString &title, const QString &message) = 0;
        virtual void writeWarning(const Base* base, const QString &title, const QString &message) = 0;

        // global
        virtual Plugin::ConfigPort *globalConfigPort() const = 0;
        virtual QList<QString> allProjectsType() const = 0;
        virtual QString getTemplatesPath() const = 0;
    };
}

#endif // SCHEDULE_H
