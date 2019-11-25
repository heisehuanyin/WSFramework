#ifndef WSPROJECTMANAGER_H
#define WSPROJECTMANAGER_H

#include "wsprojectmanager_global.h"
#include "../WsCore/include/projectmanager.h"

#include <QDomDocument>

class TreeItem : public Core::Items::DataPath
{
public:
    TreeItem(Plugin::ProjectManager *model, QDomElement resp,
             bool asLeaf = true, Core::Items::DataPath *parent = nullptr);
    virtual ~TreeItem() override;

    void addChild(TreeItem *node, int index = -1);
    void removeChild(TreeItem *node);
    void dataChangeNotify();

    QDomElement getNodeResp() const;

    // DataPath interface
public:
    virtual void registerListener(Core::Items::DataPathListener *listener) override;
    virtual void *model() const override;
    virtual Core::Items::DataPath *parent() const override;
    virtual int row() override;
    virtual int column() override;
    virtual Core::Items::DataPath *childAt(int row, int) const override;
    virtual int rowCount() const override;
    virtual int columnCount() const override;
    virtual bool isLeaf() const override;
    virtual QVariant data(int type) const override;
    virtual int indexOf(Core::Items::DataPath *child) const override;

private:
    Plugin::ProjectManager *const mgr;
    QDomElement nodeResp;
    bool _isLeaf;
    Core::Items::DataPath *parentNode;
    QList<Core::Items::DataPath*> children;
    QList<Core::Items::DataPathListener *> listener_list;

    void resetParentNode(TreeItem *node);
    void insertNotify(int index);
    void removeNotify(int index);

    // DataPath interface
public:
    virtual bool containsListener(Core::Items::DataPathListener *listener) const override;
};

class WSPROJECTMANAGERSHARED_EXPORT WsProjectManager
        :public QObject, public Plugin::ProjectManager
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGINPROJECTMANAGER)
    Q_INTERFACES(Plugin::Base Plugin::ProjectManager)

public:
    WsProjectManager();

    virtual ~WsProjectManager() override;

    // Configure interface
public:
    virtual void argumentsConfigure(const QHash<QString, QString> &xargs) override;
    virtual QList<QString> configItems() const override;
    virtual QPair<ArgsType, QList<QString> >
    ArgumentInfo(const QString &name) const override;

    // Base interface
public:
    virtual QString registerName() const override;
    virtual Plugin::PluginType pluginType() const override;
    virtual void save() override;

    // ProjectManager interface
public:
    virtual QString getValue(const QString &key) override;
    virtual void setValue(const QString &key, const QString &val) override;

    virtual ProjectManager* newManager(Core::Group::Visual* visual) override;
    virtual void openProject(const QString &project_file) override;
    virtual void newProject(const QString &project_dir) override;
    virtual QString projectRootPath() const override;
    virtual QString getFilePath(const Core::Items::DataPath *path) const override;
    virtual Core::Items::DataPath *description() const override;
    virtual void rename(const Core::Items::DataPath *path, const QString &name) override;
    virtual Core::Items::DataPath* newGroup(Core::Items::DataPath *under, const QString &name, const QString &file_path) override;
    virtual Core::Items::DataPath* newItem(Core::Items::DataPath *group, const QString &file_path, const QString &name) override;
    virtual void move(Core::Items::DataPath *item, Core::Items::DataPath *group, int index) override;
    virtual void remove(const Core::Items::DataPath *path) override;

    virtual QVariant data(const Core::Items::DataPath *path, int role) const override;

private:
    Core::Group::Visual *vgroup;
    TreeItem * root;
    QDomDocument config;
    QString project_file_path;

};

#endif // WSPROJECTMANAGER_H
