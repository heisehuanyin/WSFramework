#include "wslogwriter.h"

#include <QDir>
#include <QTime>

using namespace Plugin;

WsLogWriter::WsLogWriter()
{
    fileout = nullptr;
    textout = nullptr;
}

WsLogWriter::~WsLogWriter() {
    if(textout)
        delete textout;
    if(fileout)
        delete fileout;
}


QString WsLogWriter::registerName() const
{
    return "WsLogWriter";
}

PluginType WsLogWriter::pluginType() const
{
    return PluginType::LOGWRITER;
}


void WsLogWriter::save()
{
    this->textout->flush();
    this->fileout->flush();
}

LogWriter *WsLogWriter::newLogWriter(Plugin::Schedule *core, const QString &project_root_path)
{
    auto logPort = new WsLogWriter();

    QDir pjt(project_root_path);
    if(!pjt.exists("logs")){
        pjt.mkdir("logs");
    }
    QDir logs(project_root_path + "/logs");

    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();
    QString filename = QString("Record_%1%2%3_%4%5%6%7.logs")
                       .arg(today.year()).arg(today.month()).arg(today.day())
                       .arg(now.hour()).arg(now.minute()).arg(now.second())
                       .arg(now.msec());

    logPort->fileout = new QFile(logs.absoluteFilePath(filename));
    if(!logPort->fileout->open(QIODevice::Text|QIODevice::WriteOnly)){
        core->writeError(this, "Log文件打开失败","文件路径："+filename);
        return nullptr;
    }

    logPort->textout = new QTextStream(logPort->fileout);
    return logPort;
}

void WsLogWriter::writeLog(qint64 time_msec, const QString &title, const QString &message)
{
    (*textout) <<"[Log "<<time_msec<<"]"<< title << "/" << message << "\n";
}

void WsLogWriter::writeError(qint64 time_msec, const QString &title, const QString &message)
{
    (*textout) <<"[Error "<<time_msec<<"]"<< title << "/" << message << "\n";
}

void WsLogWriter::writeWarning(qint64 time_msec, const QString &title, const QString &message)
{
    (*textout) <<"[Warning "<<time_msec<<"]"<< title << "/" << message << "\n";
}




