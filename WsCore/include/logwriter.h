#ifndef LOGWRITER_H
#define LOGWRITER_H

#include "schedule.h"

namespace Plugin {
    class LogWriter : public Base
    {
    public:
        virtual ~LogWriter() = default;

        virtual LogWriter *newLogWriter(Plugin::Schedule *core, const QString &project_root_path) = 0;

        /**
         * @brief 向logport写入log信息
         * @param time_msec 自1970-01-01T00:00:00.000起，经过的毫秒数UTC
         * @param title
         * @param message
         */
        virtual void writeLog(qint64 time_msec, const QString &title, const QString &message) = 0;
        virtual void writeError(qint64 time_msec, const QString &title, const QString &message) = 0;
        virtual void writeWarning(qint64 time_msec, const QString &title, const QString &message) = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGIN_LOGWRITER "wsframework.plugin-set.logwriter"
Q_DECLARE_INTERFACE(Plugin::LogWriter, PLUGIN_LOGWRITER)

QT_END_NAMESPACE

#endif // LOGWRITER_H
