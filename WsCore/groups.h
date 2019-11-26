#ifndef GROUPS_H
#define GROUPS_H

#include "include/datatype.h"
#include "include/pluginsmanager.h"

class FGroup : public Core::Group::File
{
public:
    FGroup(const QString &filePath, Plugin::PluginsManager *mgr, Core::Group::Project *pjt);
    virtual ~FGroup() override;

    // File interface
public:
    virtual Core::Group::Project *projectGroup() const override;
    virtual QString filePath() const override;
    virtual QString fileName() const override;
    virtual QList<Plugin::Base *> head() const override;
    virtual bool contains(Plugin::Base *target) const override;
    virtual Plugin::Schedule *schedule() const override;
    virtual QList<Plugin::Base*> upStream(Plugin::Base *target) const override;
    virtual Plugin::Base *downStream(Plugin::Base *target) override;

private:
    Core::Group::Project *const project_group;
    Plugin::PluginsManager *const plugins_mgr;

    const QString &target_file;
    QList<Plugin::Base*> headPlugins;
    QList<Plugin::Base*> all_plugins;

    // Process interface
public:
    virtual void openTarget() override;
    virtual void save() override;

};

class PGroup : public Core::Group::Project
{
public:
    /**
     * @brief Project-Group
     * @param v visual-package
     * @param projectmgr 未打开projectmgr，不管理
     * @param pluginsmgr 不管理
     */
    PGroup(Core::Group::Visual *v, Plugin::PluginsManager *mgr);
    virtual ~PGroup() override;

    // Project interface
public:
    virtual void openTarget(const QString &project_file) override;
    virtual void newTarget(const QString &pluginName, const QString &project_root_path) override;

    virtual Core::Group::Visual *visualSet() const override;
    virtual Plugin::Schedule *schedule() const override;
    virtual bool contains(Plugin::Base *target) override;
    virtual Plugin::ProjectManager *projectManager() const override;
    virtual Plugin::LogWriter *logWriter() const override;
    virtual Plugin::ConfigPort *configPort() const override;
    virtual QList<Plugin::Base *> getEnhancePlugins() const override;
    virtual QString projectRootPath() const override;
    virtual Core::Group::File *openFile(const QString &filePath) override;
    virtual QList<QString> openedFiles() const override;
    virtual Core::Group::File *getFile(const QString &filePath) const override;

private:
    Core::Group::Visual *const vgroup;
    Plugin::PluginsManager *const pluginsmgr;
    Plugin::ProjectManager *projectmgr;
    Plugin::LogWriter *logwriter;
    Plugin::ConfigPort *configport;
    QList<Plugin::Base*> enhance_plugs;

    QList<FGroup*> opend_files;

    // Project interface
public:
    virtual void saveTarget() override;
    virtual void saveFile(Core::Group::File *file) override;
    virtual void closeFile(Core::Group::File *file) override;

    // Project interface
public:
    virtual QList<const Plugin::FileGenerator *> allGenerators() const override;
    virtual void newFileWithin(const QString &parentDir, const QString &plgName,
                         const QHash<QString, QString> &xargs) override;
};


class ProjectTree : public QAbstractItemModel, public Core::Items::DataPathListener
{
public:
    ProjectTree(Core::Group::Visual *visual);
    virtual ~ProjectTree() override;

    void appendProject(Core::Items::DataPath *project_root);
    void removeProject(Core::Items::DataPath *project_root);
    Core::Items::DataPath *queryPath(const QModelIndex &index) const;

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool hasChildren(const QModelIndex &parent) const override;

    // DataPathListener interface
public:
    virtual void dataChanged(Core::Items::DataPath *item) override;
    virtual void nodeInsert(Core::Items::DataPath *group, int row, int column) override;
    virtual void nodeRemove(Core::Items::DataPath *group, int row, int column) override;

private:
    DataPathListener *this_listener = this;
    QList<Core::Items::DataPath*> project_roots;
    Core::Group::Visual *const visual_group;
    /**
     * @brief 查询指定Item的index
     * @param item
     * @return item-index
     */
    QModelIndex queryIndeces(Core::Items::DataPath* item);
};

class VGroup : public Core::Group::Visual
{
public:
    /**
     * @brief 新建一个visual-set插件模型
     * @param core 不管
     * @param plgsm 不管
     */
    VGroup(Plugin::Schedule *core, Plugin::PluginsManager *plgsm);
    virtual ~VGroup() override;

    // VisualSet interface
public:
    virtual Plugin::Schedule *schedule() const override;
    virtual Plugin::MenuManager *getMenuManager() const override;
    virtual Plugin::Window *getWindow() const override;
    /**
     * @brief 打开窗口，如果窗口未打开，新建窗口实例
     */
    void openWindow();
    virtual bool contains(Plugin::Base *target) override;
    virtual QList<Plugin::Base*> getIndependentTools() const override;
    virtual Core::Group::Project *openProject(const QString &project_file) override;
    virtual Core::Group::Project *newProject(const QString &plugin, const QString &project_root_path) override;
    virtual QList<QString> aliveProjects() const override;
    virtual QString activedProjectRootPath() const override;
    virtual Core::Group::Project *getProject(const QString &project_dir) const override;
    virtual void activeProject(Core::Group::Project *project) override;
    virtual QAbstractItemModel *getProjectsTree() const override;

private:
    Plugin::Schedule *const core_schedule;
    Plugin::PluginsManager *const plgs_mgr;
    ProjectTree *tree;
    Plugin::MenuManager *menu_mgr;
    Plugin::Window *window;

    QList<Plugin::Base*> independent_tools;
    QList<PGroup*> projects;
    Core::Group::Project * actived_project;

    // Visual interface
public:
    virtual void saveProject(Core::Group::Project *pjt) override;
    virtual void closeProject(Core::Group::Project *pjt) override;
    virtual void refreshVisibleMenus() override;
    virtual Core::Items::DataPath *nodeConvert(const QModelIndex &index) const override;
};

#endif // GROUPS_H
