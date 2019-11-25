#ifndef WSLOGWRITER_H
#define WSLOGWRITER_H

#include "wslogwriter_global.h"
#include "../WsCore/include/logwriter.h"

#include <QFile>
#include <QTextStream>

class WSLOGWRITERSHARED_EXPORT WsLogWriter
        : public QObject, public Plugin::LogWriter
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_LOGWRITER)
    Q_INTERFACES(Plugin::LogWriter Plugin::Base)

public:
    WsLogWriter();
    virtual ~WsLogWriter() override;

    // Configure interface
public:
    virtual void argumentsConfigure(const QHash<QString,QString> &xargs) override;
    virtual QList<QString> configItems() const override;
    virtual QPair<ArgsType, QList<QString> > ArgumentInfo(const QString &name) const override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

    // LogWriter interface
public:
    virtual LogWriter* newLogWriter(Plugin::Schedule *core, const QString &project_root_path) override;

    virtual void writeLog(qint64 time_msec, const QString &title, const QString &message) override;
    virtual void writeError(qint64 time_msec, const QString &title, const QString &message) override;
    virtual void writeWarning(qint64 time_msec, const QString &title, const QString &message) override;

private:
    QFile * fileout;
    QTextStream *textout;
};

#endif // WSLOGWRITER_H
