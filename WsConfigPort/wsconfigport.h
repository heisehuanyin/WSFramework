#ifndef WSCONFIGPORT_H
#define WSCONFIGPORT_H

#include "wsconfigport_global.h"
#include "../WsCore/include/configport.h"

#include <QDomDocument>

class WSCONFIGPORTSHARED_EXPORT WsConfigPort
        : public QObject, public Plugin::ConfigPort
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_CONFIGPORT)
    Q_INTERFACES(Plugin::ConfigPort Plugin::Base)

public:
    WsConfigPort();
    virtual ~WsConfigPort() override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

    // ConfigPort interface
public:
    virtual ConfigPort *newConfigPort(Plugin::Schedule *schedule_core) override;
    virtual QString filePath() const override;

    virtual void openFile(const QString &file_path) override;
    virtual void newFile(const QString &project_root_path) override;

    virtual void setValue(const Base *base, const QString &key, const QString &val) override;
    virtual QString getValue(const Base *base, const QString &key) const override;

    virtual void setList(const Base *base, const QString &name, const QList<QString> &list) override;
    virtual QList<QString> getList(const Base *base, const QString &name) const override;

    virtual void setMap(const Base *base, const QString &name, const QHash<QString, QString> &map) override;
    virtual QHash<QString, QString> getMap(const Base *base, const QString &name) const override;

private:
    QDomDocument configure_doc;
    Plugin::Schedule *schedule_core;
    QString file_path;

    /**
     * @brief 检索直接子节点，满足tagName[attr_name=target_value]
     * @param node 父节点
     * @param tag_name tagName
     * @param attr_name 指定属性名称
     * @param target_value 指定属性值
     * @return
     */
    QDomElement locateChildElement(QDomElement &node, const QString &tag_name,
                                   const QString &attr_name, const QString &target_value) const;
};



#endif // WSCONFIGPORT_H
