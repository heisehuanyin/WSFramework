#include "wsmenumanager.h"
#include "newdialog.h"

#include <QFileDialog>

WsMenuManager::WsMenuManager()
    :file_new(new QAction("新建文件")),
      file_groups(new QAction("组织集合")),
      file_append(new QAction("添加文件")),
      file_save(new QMenu("保存文件")),
      file_close(new QMenu("关闭文件")),
      file_history(new QMenu("文件历史")),
      project_new(new QMenu("新建项目")),
      project_open(new QAction("打开项目")),
      project_save(new QMenu("保存项目")),
      project_close(new QMenu("关闭项目")),
      project_history(new QMenu("历史项目")),
      project_print(new QAction("打印")),
      exit(new QAction("退出"))
{
    auto files = new QMenu(tr("文件"));
    files->addAction(file_new);
    files->addAction(file_groups);
    files->addAction(file_append);
    files->addSeparator();
    files->addMenu(file_save);
    files->addMenu(file_close);
    files->addMenu(file_history);
    files->addSeparator();
    files->addMenu(project_new);
    files->addAction(project_open);
    files->addMenu(project_save);
    files->addMenu(project_close);
    files->addMenu(project_history);
    files->addSeparator();
    files->addAction(project_print);
    files->addSeparator();
    files->addAction(exit);


    auto edit = new QMenu(tr("编辑"));
    auto function = new QMenu(tr("功能"));
    auto tools = new QMenu(tr("工具"));
    auto plugins = new QMenu(tr("插件"));
    auto views = new QMenu(tr("视图"));
    auto help = new QMenu(tr("帮助"));

    menus.append(files);
    menus.append(edit);
    menus.append(function);
    menus.append(tools);
    menus.append(plugins);
    menus.append(views);
    menus.append(help);

    // 新建文件关联
    connect(file_new,       &QAction::triggered,this,   &WsMenuManager::files_fileNew);

    // 新建项目动作
    connect(project_new,    &QMenu::triggered,  this,   &WsMenuManager::files_projectNew);
    connect(project_open,   &QAction::triggered,this,   &WsMenuManager::files_projectOpen);
    connect(project_save,   &QMenu::triggered,  this,   &WsMenuManager::files_projectSave);
    connect(project_close,  &QMenu::triggered,  this,   &WsMenuManager::files_projectClose);
}

WsMenuManager::~WsMenuManager(){
    for (auto item : menus) {
        delete item;
    }
}

QString WsMenuManager::registerName() const{
    return "WsMenuManager";
}

Plugin::PluginType WsMenuManager::pluginType() const{
    return Plugin::MENUSMANAGER;
}


void WsMenuManager::save(){

}

Plugin::MenuManager *WsMenuManager::newMenuManager(Core::Group::Visual *core){
    auto ret = new WsMenuManager();
    ret->visual = core;
    return ret;
}

int WsMenuManager::groupCount() const{
    return menus.size();
}

QMenu *WsMenuManager::menuGroup(int index){
    if(index >= 0 && index < menus.size())
        return menus.at(index);

    return nullptr;
}

void WsMenuManager::registerAction(QAction *action, Plugin::Base *host)
{
    if(!actions_map.contains(host))
        actions_map.insert(host, new QList<QPair<QAction*, QMenu*>>());
    auto target_group = actions_map.value(host);

    switch (host->pluginType()) {
        case Plugin::TEXTVIEW:
        case Plugin::ITEMSVIEW:{
                target_group->append(qMakePair(action, menus.at(1))); // 编辑
            }break;
        case Plugin::GRAPHICSFRONTEND:{
                target_group->append(qMakePair(action, menus.at(5))); // 视图
            }break;
        case Plugin::INDEPENDENTTOOLS:{
                target_group->append(qMakePair(action, menus.at(3))); // 工具
            }break;
        case Plugin::PROJECTMANAGER:
        case Plugin::PROJECTENHANCE:{
                target_group->append(qMakePair(action, menus.at(2))); // 功能
            }break;
        default:{
                target_group->append(qMakePair(action, menus.at(4))); // 插件
            }break;
    }
}

void WsMenuManager::menuActive(const QList<Plugin::Base *> &actived_set)
{
    // 插件注册菜单
    for (auto plugin : actived_stack) {
        if(!actions_map.contains(plugin))
            continue;

        auto plist = actions_map.value(plugin);
        for(auto item : *plist){
            item.second->removeAction(item.first);
        }
    }
    actived_stack = actived_set;
    for (auto plugin : actived_stack) {
        if(!actions_map.contains(plugin))
            continue;

        auto plist = actions_map.value(plugin);
        for(auto item : *plist){
            item.second->addAction(item.first);
        }
    }

    // 通用菜单刷新
    this->refreshFilesMenu();
}

void WsMenuManager::removeRelatedMenus(Plugin::Base *plugin) {
    if(!actions_map.contains(plugin))
        return;

    auto list = actions_map.value(plugin);
    actions_map.remove(plugin);
    delete list;
    actived_stack.removeAll(plugin);
}

void WsMenuManager::files_fileNew()
{
    auto rootPath = visual->activedProjectRootPath();
    auto pjt = visual->getProject(rootPath);
    NewDialog(pjt, visual->getWindow()->getWidget()).exec();
}

void WsMenuManager::files_projectNew(QAction *act)
{
    auto pjt_type = act->text();
    auto target_dir = QFileDialog::getExistingDirectory(
                          visual->getWindow()->getWidget(),
                          tr("获取项目根目录文件夹"));

    if(target_dir == "")
        return;

    visual->newProject(pjt_type, target_dir);
    visual->refreshVisibleMenus();
}

void WsMenuManager::files_projectOpen()
{
    auto target_file = QFileDialog::getOpenFileName(
                           visual->getWindow()->getWidget(),
                           tr("打开指定项目文件"));

    if(target_file == "")
        return;

    visual->openProject(target_file);
    visual->refreshVisibleMenus();
}

void WsMenuManager::files_projectSave(QAction *act)
{
    auto project_root_path = act->text();
    auto pjt = visual->getProject(project_root_path);
    visual->saveProject(pjt);
}

void WsMenuManager::files_projectClose(QAction *act)
{
    auto project_root_path = act->text();
    auto pjt = visual->getProject(project_root_path);
    visual->closeProject(pjt);
    visual->refreshVisibleMenus();
}

void WsMenuManager::refreshFilesMenu()
{
    // 新建文件模板刷新
    QDir tfile(visual->schedule()->getTemplatesPath());
    auto list = tfile.entryInfoList(QDir::Files);

    // 新建项目刷新
    project_new->clear();
    auto newpjts2 = visual->schedule()->allProjectsType();
    for (auto item : newpjts2) {
        project_new->addAction(item);
    }
    // 活跃项目刷新
    project_close->clear();
    project_save->clear();
    auto active_pjts = visual->aliveProjects();
    for (auto item_pjt : active_pjts) {
        project_close->addAction(item_pjt);
        project_save->addAction(item_pjt);
    }
}
