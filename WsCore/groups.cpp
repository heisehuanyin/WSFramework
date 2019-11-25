#include "groups.h"

#include <QFile>
#include <QFileInfo>

using namespace Plugin;


FGroup::FGroup(const QString &filePath, PluginsManager *mgr, Core::Group::Project *pjt)
    :project_group(pjt), plugins_mgr(mgr), target_file(filePath){}

FGroup::~FGroup(){
    auto heads = head();
    for (auto h : heads) {
        plugins_mgr->unloadRelatedPlugins(h);
    }
}

Core::Group::Project *FGroup::projectGroup() const{
    return project_group;
}

QString FGroup::filePath() const{
    return target_file;
}

QString FGroup::fileName() const
{
    return QFile(target_file).fileName();
}

QList<Base *> FGroup::head() const{
    return headPlugins;
}

bool FGroup::contains(Base *target) const{
    return all_plugins.contains(target);
}

Schedule *FGroup::schedule() const{
    return project_group->schedule();
}

QList<Base *> FGroup::upStream(Base *target) const{
    return QList<Plugin::Base*>();
}

Base *FGroup::downStream(Base *target){
    return nullptr;
}

void FGroup::openTarget()
{

}

void FGroup::save()
{

}

PGroup::PGroup(Core::Group::Visual *v, PluginsManager *mgr)
    :vgroup(v), pluginsmgr(mgr), projectmgr(nullptr),
      logwriter(nullptr),configport(nullptr){}

PGroup::~PGroup(){
    for (auto f : opend_files) {
        delete f;
    }

    pluginsmgr->unloadRelatedPlugins(projectmgr);
}

void PGroup::openTarget(const QString &project_file)
{
    QFileInfo f(project_file);
    if(!f.exists()){
        vgroup->schedule()->writeError(vgroup->schedule(), "项目打开错误", "选定文件不存在");
        return;
    }

    auto gcfg = vgroup->schedule()->globalConfigPort();
    auto project_resolve = gcfg->getMap(vgroup->schedule(), "project_resolve");
    auto suffix = f.suffix();
    QString manager_type = "";
    if(project_resolve.contains(suffix))
        manager_type = project_resolve.value(suffix);
    else
        manager_type = "WsProjectManager";

    projectmgr = pluginsmgr->newProjectManager(vgroup->getMenuManager(), manager_type, vgroup);

    // project-open
    projectmgr->openProject(project_file);

    // project-config-new
    auto config_plugin = projectmgr->getValue("project-configport");
    if(config_plugin != "")
        configport = pluginsmgr->newConfigPort(projectmgr, config_plugin, vgroup->schedule());
    if(configport == nullptr){
        config_plugin = gcfg->getValue(projectmgr, "global-configport");
        if(config_plugin == "") config_plugin = "WsConfigPort";
        configport = pluginsmgr->newConfigPort(projectmgr, config_plugin, vgroup->schedule());
    }

    auto cfgpath = projectmgr->getValue("config-file-path");
    if(cfgpath == "")
        configport->newFile(projectmgr->projectRootPath());
    else
        configport->openFile(cfgpath);

    // project-log-new
    auto log_plugin = projectmgr->getValue("project-logwriter");
    if(log_plugin != "")
        logwriter = pluginsmgr->newLogWriter(projectmgr, log_plugin, projectmgr->projectRootPath(), vgroup->schedule());

    if(logwriter == nullptr){
        log_plugin = gcfg->getValue(projectmgr, "global-logwriter");
        if(log_plugin == "") log_plugin = "WsLogWriter";
        logwriter = pluginsmgr->newLogWriter(projectmgr, log_plugin, projectmgr->projectRootPath(), vgroup->schedule());
    }
}

void PGroup::newTarget(const QString &pluginName, const QString &project_root_path)
{
    projectmgr = pluginsmgr->newProjectManager(vgroup->getMenuManager(), pluginName, vgroup);

    // project-new
    auto gcfg = vgroup->schedule()->globalConfigPort();
    projectmgr->newProject(project_root_path);

    // project-config-new
    auto plugin = gcfg->getValue(projectmgr, "global-configport");
    if(plugin == "") plugin = "WsConfigPort";
    projectmgr->setValue("project-configport", plugin);

    configport = pluginsmgr->newConfigPort(projectmgr, plugin, vgroup->schedule());
    configport->newFile(project_root_path);
    projectmgr->setValue("config-file-path", configport->filePath());


    // project-log-new
    auto log_plugin = gcfg->getValue(projectmgr, "global-configport");
    if(log_plugin == "") log_plugin = "WsLogWriter";
    projectmgr->setValue("project-logwriter", log_plugin);

    logwriter = pluginsmgr->newLogWriter(projectmgr, log_plugin, projectmgr->projectRootPath(), vgroup->schedule());

    // save
    projectmgr->save();
    configport->save();
    logwriter->save();
}

Core::Group::Visual *PGroup::visualSet() const{
    return vgroup;
}

Schedule *PGroup::schedule() const{
    return vgroup->schedule();
}

bool PGroup::contains(Base *target){
    if(projectmgr == target)
        return true;
    if(logwriter == target)
        return true;
    if(configport == target)
        return true;
    if(enhance_plugs.contains(target))
        return true;
    for (auto f : opend_files) {
        if(f->contains(target))
            return true;
    }

    return false;
}

ProjectManager *PGroup::projectManager() const{
    return projectmgr;
}

LogWriter *PGroup::logWriter() const{
    return logwriter;
}

ConfigPort *PGroup::configPort() const{
    return configport;
}

QList<Base *> PGroup::getEnhancePlugins() const{
    return enhance_plugs;
}

QString PGroup::projectRootPath() const{
    return projectmgr->projectRootPath();
}

Core::Group::File *PGroup::openFile(const QString &filePath){
    auto file = new FGroup(filePath, this->pluginsmgr, this);
    opend_files.append(file);
    // TODo else-process

    return file;
}

QList<QString> PGroup::openedFiles() const{
    QList<QString> flist;
    for (auto f : opend_files) {
        flist.append(f->filePath());
    }
    return flist;
}

Core::Group::File *PGroup::getFile(const QString &filePath) const{
    for (auto f : opend_files) {
        if(f->filePath() == filePath)
            return f;
    }
    return nullptr;
}

void PGroup::saveTarget()
{

}

void PGroup::saveFile(Core::Group::File *file)
{

}

void PGroup::closeFile(Core::Group::File *file)
{

}

VGroup::VGroup(Schedule *core, PluginsManager *plgsm)
    :core_schedule(core), plgs_mgr(plgsm), tree(new ProjectTree(this)),
      menu_mgr(nullptr), window(nullptr), actived_project(nullptr)
{
    menu_mgr = plgs_mgr->newMenuManager(core_schedule, "WsMenuManager", this);
}

VGroup::~VGroup()
{
    // delete project-page
    for (auto pjt : projects) {
        delete pjt;
    }
    projects.clear();

    // delete description
    if(tree){
        delete tree;
        tree = nullptr;
    }

    if(menu_mgr){
        this->plgs_mgr->unloadRelatedPlugins(menu_mgr);
        menu_mgr = nullptr;
    }
    if(window){
        this->plgs_mgr->unloadRelatedPlugins(window);
        window = nullptr;
    }
}

Schedule *VGroup::schedule() const{
    return core_schedule;
}

MenuManager *VGroup::getMenuManager() const{
    return menu_mgr;
}

Window *VGroup::getWindow() const{
    return window;
}

void VGroup::openWindow()
{
    if(!window){
        window = plgs_mgr->newWindow(core_schedule, "WsWindow", this);
        window->loadMenuManager(menu_mgr);
    }

    window->show();
}

bool VGroup::contains(Base *target){
    if(target == menu_mgr)
        return true;
    if(target == window)
        return true;
    if(independent_tools.contains(target))
        return true;
    for (auto pjt : projects) {
        if(pjt->contains(target))
            return true;
    }
    return false;
}

QList<Base *> VGroup::getIndependentTools() const{
    return independent_tools;
}

Core::Group::Project *VGroup::openProject(const QString &project_file)
{
    auto pjt_page = new PGroup(this, this->plgs_mgr);
    pjt_page->openTarget(project_file);

    projects.append(pjt_page);
    tree->appendProject(pjt_page->projectManager()->description());
    this->actived_project = pjt_page;
    tree->dataChanged(pjt_page->projectManager()->description());

    return pjt_page;
}

Core::Group::Project *VGroup::newProject(const QString &plugin, const QString &project_root_path)
{
    auto pjt_page = new PGroup(this, this->plgs_mgr);
    pjt_page->newTarget(plugin, project_root_path);

    projects.append(pjt_page);
    tree->appendProject(pjt_page->projectManager()->description());
    actived_project = pjt_page;
    tree->dataChanged(pjt_page->projectManager()->description());

    return pjt_page;
}

QList<QString> VGroup::aliveProjects() const{
    QList<QString> alives;
    for(auto root : projects){
        alives.append(root->projectRootPath());
    }
    return alives;
}

QString VGroup::activedProject() const{
    if(actived_project != nullptr)
        return actived_project->projectRootPath();
    else
        return "";
}

Core::Group::Project *VGroup::getProject(const QString &project_dir) const{
    for (auto pjt : projects) {
        if(pjt->projectRootPath() == project_dir)
            return pjt;
    }
    return nullptr;
}

void VGroup::activeProject(Core::Group::Project *project)
{
    this->actived_project = project;
    this->tree->dataChanged(project->projectManager()->description());
}

QAbstractItemModel *VGroup::getProjectsTree() const{
    return tree;
}

void VGroup::saveProject(Core::Group::Project *pjt)
{
    pjt->saveTarget();
}

void VGroup::closeProject(Core::Group::Project *pjt)
{
    tree->removeProject(pjt->projectManager()->description());
    projects.removeAll(static_cast<PGroup*>(pjt));
    delete pjt;

    if(!projects.size()){
        actived_project = nullptr;
    }
    else {
        actived_project = projects.at(0);
        tree->dataChanged(actived_project->projectManager()->description());
    }
}

void VGroup::refreshVisibleMenus()
{
    auto list = plgs_mgr->downstreamPlugins(window);
    list.append(window);

    for(auto t : independent_tools){
        list.append(t);
        list.append(plgs_mgr->downstreamPlugins(t));
    }
    if(actived_project){
        auto pjt = actived_project->projectManager();
        list.append(pjt);
        list.append(plgs_mgr->downstreamPlugins(pjt));
    }

    menu_mgr->menuActive(list);
}

Core::Items::DataPath *VGroup::nodeConvert(const QModelIndex &index) const
{
    return tree->queryPath(index);
}

ProjectTree::ProjectTree(Core::Group::Visual *visual)
    :visual_group(visual){}

ProjectTree::~ProjectTree() {}

void ProjectTree::appendProject(Core::Items::DataPath *project_root)
{
    if(!project_roots.contains(project_root)) {
        this->beginInsertRows(QModelIndex(),rowCount(QModelIndex()),rowCount(QModelIndex()));
        project_root->registerListener(this_listener);
        project_roots.append(project_root);
        this->endInsertRows();
    }
}

void ProjectTree::removeProject(Core::Items::DataPath *project_root)
{
    if(project_roots.contains(project_root)){
        auto row = project_roots.indexOf(project_root);
        this->beginRemoveRows(QModelIndex(), row, row);
        project_roots.removeAll(project_root);
        this->endRemoveRows();
    }
}

Core::Items::DataPath *ProjectTree::queryPath(const QModelIndex &index) const
{
    if(!index.isValid())
        return nullptr;

    return static_cast<Core::Items::DataPath*>(index.internalPointer());
}

QModelIndex ProjectTree::index(int row, int column, const QModelIndex &parent) const{
    // 范围之外
    if(column != 0 || row < 0 || row >= rowCount(parent))
        return QModelIndex();

    // 获取project-node
    if(parent == QModelIndex()){
        auto item = project_roots.at(row);
        return createIndex(row, 0, item);
    }

    auto p_ptr = static_cast<Core::Items::DataPath*>(parent.internalPointer());
    auto child = p_ptr->childAt(row);
    if(!child->containsListener(this_listener))
        child->registerListener(this_listener);
    return createIndex(row, 0, child);
}

QModelIndex ProjectTree::parent(const QModelIndex &child) const{
    // 范围外或者root节点
    if(!child.isValid())
        return QModelIndex();

    // project-node
    auto pointer = child.internalPointer();
    auto realptr = static_cast<Core::Items::DataPath*>(pointer);
    if(project_roots.contains(realptr))
        return QModelIndex();

    // project-node 直接子节点
    auto parent = realptr->parent();
    if(project_roots.contains(parent))
        return createIndex(project_roots.indexOf(parent), 0, parent);

    // 直接子节点之下其他节点
    auto p_parent = parent->parent();
    return createIndex(p_parent->indexOf(parent), 0, parent);
}

int ProjectTree::rowCount(const QModelIndex &parent) const{
    if(parent == QModelIndex())
        return project_roots.size();

    auto pointer = parent.internalPointer();
    auto realp = static_cast<Core::Items::DataPath*>(pointer);
    return realp->rowCount();
}

int ProjectTree::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 1;
}

QVariant ProjectTree::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    auto ptr = index.internalPointer();
    auto real = static_cast<Core::Items::DataPath*>(ptr);

    auto pjt_identifer = visual_group->activedProject();
    auto pjt_ins = visual_group->getProject(pjt_identifer);
    if(role == Qt::BackgroundRole && pjt_ins &&
       real == pjt_ins->projectManager()->description())
        return QColor(0xf0,0xf0,0xf0);

    return real->data(role);
}

QModelIndex ProjectTree::queryIndeces(Core::Items::DataPath *item)
{
    QList<Core::Items::DataPath*> path;
    do{
        path.insert(0, item);
        if(project_roots.contains(item))
            break;
        item = item->parent();
    }while (true);

    QModelIndex ptIndex = QModelIndex();
    while (path.size() > 0) {
        auto target = path.takeAt(0);
        auto tIndex = index(0, 0, ptIndex);

        while (true) {
            if(target != tIndex.internalPointer()){
                tIndex = tIndex.sibling(tIndex.row()+1, 0);
                continue;
            }
            ptIndex = tIndex;
            break;
        }
    }

    return ptIndex;
}

void ProjectTree::dataChanged(Core::Items::DataPath *item){
    auto index = queryIndeces(item);
    emit QAbstractItemModel::dataChanged(index, index);
}

void ProjectTree::nodeInsert(Core::Items::DataPath *group, int row, int)
{
    auto index = queryIndeces(group);
    this->beginInsertRows(index, row, row);
    this->endInsertRows();
}

void ProjectTree::nodeRemove(Core::Items::DataPath *group, int row, int){
    auto index = queryIndeces(group);
    this->beginRemoveRows(index, row, row);
    this->endRemoveRows();
}

bool ProjectTree::hasChildren(const QModelIndex &parent) const{
    if(parent.isValid() && parent.internalPointer() &&
       static_cast<Core::Items::DataPath*>(parent.internalPointer())->isLeaf())
        return false;
    return true;
}
