#include "wsprojectmanager.h"
#include "../WsCore/include/menumanager.h"

#include <QStyle>
#include <QWidget>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>

TreeItem::TreeItem(Plugin::ProjectManager *model, QDomElement resp, bool asLeaf, Core::Items::DataPath *parent)
    :mgr(model), nodeResp(resp),
      _isLeaf(asLeaf), parentNode(parent){}

TreeItem::~TreeItem(){
    for (auto c:children) {
        delete c;
    }
}

void TreeItem::addChild(TreeItem *node, int index){
    if(!children.contains(node)){
        node->resetParentNode(this);

        if(index >= 0 && index < children.size()){
            children.insert(index, node);

            auto fchild = nodeResp.firstChildElement();
            auto shadow_index = index;
            while (shadow_index >= 0) {
                if(!shadow_index)
                    break;
                shadow_index--;
                fchild = fchild.nextSiblingElement();
            }
            this->nodeResp.insertBefore(node->nodeResp, fchild);
            this->insertNotify(index);
        }
        else {
            children.append(node);
            this->nodeResp.appendChild(node->nodeResp);
            this->insertNotify(children.size()-1);
        }
    }
}

void TreeItem::removeChild(TreeItem *node){
    auto index = children.indexOf(node);

    if(index != -1){
        children.removeAt(index);
        this->nodeResp.removeChild(node->nodeResp);
        removeNotify(index);
    }
}

void TreeItem::dataChangeNotify(){
    for (auto l:listener_list) {
        l->dataChanged(this);
    }
}

void TreeItem::registerListener(Core::Items::DataPathListener *listener)
{
    if(!listener_list.contains(listener))
        listener_list.append(listener);
}

void *TreeItem::model() const {
    return mgr;
}

Core::Items::DataPath *TreeItem::parent() const{
    return parentNode;
}

int TreeItem::row() {
    return children.indexOf(this);
}

int TreeItem::column() {
    return 0;
}

Core::Items::DataPath *TreeItem::childAt(int row, int) const{
    if(row >=0 && row < children.size())
        return children.at(row);
    return nullptr;
}

int TreeItem::rowCount() const{
    return children.size();
}

int TreeItem::columnCount() const{
    return 1;
}

bool TreeItem::isLeaf() const{
    return _isLeaf;
}

QVariant TreeItem::data(int type) const{
    return mgr->data(this, type);
}

int TreeItem::indexOf(Core::Items::DataPath *child) const{
    if(child->parent() != nullptr)
        return -1;
    return children.indexOf(child);
}

QDomElement TreeItem::getNodeResp() const
{
    return nodeResp;
}

void TreeItem::resetParentNode(TreeItem *node){
    this->parentNode = node;
}

void TreeItem::insertNotify(int index){
    for (auto l:listener_list) {
        l->nodeInsert(this, index, 0);
    }
}

void TreeItem::removeNotify(int index){
    for (auto l:listener_list) {
        l->nodeRemove(this, index, 0);
    }
}

bool TreeItem::containsListener(Core::Items::DataPathListener *listener) const
{
    return listener_list.contains(listener);
}



WsProjectManager::WsProjectManager(){
    this->config.appendChild(config.createProcessingInstruction("xml","version='1.0'"));
    auto project = config.createElement("project");
    project.setAttribute("name", "新建项目");
    this->config.appendChild(project);

    this->vgroup = nullptr;
    this->project_file_path = "";
    this->root = new TreeItem(this, config.documentElement(), false);
}

WsProjectManager::~WsProjectManager(){
    if(root) delete root;
}


QString WsProjectManager::registerName() const{
    return "WsProjectManager";
}

Plugin::PluginType WsProjectManager::pluginType() const{
    return Plugin::PROJECTMANAGER;
}


void WsProjectManager::save(){
    QFile f(project_file_path);
    if(!f.open(QIODevice::Text | QIODevice::WriteOnly)){
        vgroup->schedule()->writeError(this, "项目保存错误", "项目文件无法打开");
        return;
    }
    QTextStream tout(&f);
    tout << config.toString(4);
    tout.flush();
    f.flush();
    f.close();
}

QString WsProjectManager::getValue(const QString &key)
{
    auto root = config.documentElement();
    auto children = root.elementsByTagName("config-list");
    if(children.size() == 0){
        auto newelm = config.createElement("config-list");
        root.appendChild(newelm);
    }

    auto config_list = root.elementsByTagName("config-list").at(0).toElement();
    auto kvpairs = config_list.elementsByTagName("kvitem");

    for(auto i=0; i<kvpairs.size(); ++i){
        auto elm = kvpairs.at(i).toElement();
        if(elm.attribute("key") == key)
            return elm.attribute("value");
    }

    return QString();
}

void WsProjectManager::setValue(const QString &key, const QString &val)
{
    auto root = config.documentElement();
    auto children = root.elementsByTagName("config-list");
    if(children.size() == 0){
        auto newelm = config.createElement("config-list");
        root.appendChild(newelm);
    }

    auto config_list = root.elementsByTagName("config-list").at(0).toElement();
    auto kvpairs = config_list.elementsByTagName("kvitem");

    for (auto i=0; i<kvpairs.size(); ++i) {
        auto elm = kvpairs.at(i).toElement();
        if(elm.attribute("key") == key){
            elm.setAttribute("value", val);
            return;
        }
    }

    auto newelm = config.createElement("kvitem");
    newelm.setAttribute("key", key);
    newelm.setAttribute("value", val);

    config_list.appendChild(newelm);
}

Plugin::ProjectManager *WsProjectManager::newManager(Core::Group::Visual *visual){
    auto n = new WsProjectManager();
    n->vgroup = visual;

    auto mgr = visual->getMenuManager();
    mgr->registerAction(new QAction("hello"), n);

    return n;
}

void WsProjectManager::openProject(const QString &project_file){
    this->project_file_path = project_file;

    QFile ff(project_file);
    if (!ff.exists()) {
        vgroup->schedule()->writeError(this, "项目配置文件打开错误", "指定文件不存在");
        return;
    }

    if(!ff.open(QIODevice::ReadOnly| QIODevice::Text)){
        vgroup->schedule()->writeError(this, "项目配置文件打开错误", "指定文件无法打开");
        return;
    }

    this->config.setContent(&ff, false);
    delete root;
    root = new TreeItem(this, config.documentElement(), false);

    ff.close();
}

void WsProjectManager::newProject(const QString &project_dir)
{
    QDir ff(project_dir);
    if(!ff.exists()){
        vgroup->schedule()->writeError(this, "项目新建错误", "指定项目根目录不存在");
        return;
    }
    auto list = ff.entryInfoList(QStringList()<<"*.project", QDir::Files);
    if(list.size() > 0){
        vgroup->schedule()->writeError(this, "项目配置文件新建错误", "配置文件已经存在。");
        return;
    }

    this->project_file_path = QFileInfo(project_dir).absoluteFilePath() + "/config.project";
    QFile f(project_file_path);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        vgroup->schedule()->writeError(this, "项目配置文件新建错误", "配置文件无法打开并写入");
        return;
    }

    QTextStream tout(&f);
    tout << config.toString(4);
    tout.flush();
    f.flush();
    f.close();
}

QString WsProjectManager::projectRootPath() const{
    return QFileInfo(project_file_path).absolutePath();
}

QString WsProjectManager::getFilePath(const Core::Items::DataPath *path) const{
    return static_cast<TreeItem*>(const_cast<Core::Items::DataPath*>(path))
            ->getNodeResp().attribute("file");
}

Core::Items::DataPath *WsProjectManager::description() const{
    return root;
}

void WsProjectManager::rename(const Core::Items::DataPath *path, const QString &name){
    auto dom = static_cast<const TreeItem*>(path)->getNodeResp();
    dom.setAttribute("name", name);

    auto d = const_cast<Core::Items::DataPath*>(path);
    static_cast<TreeItem*>(d)->dataChangeNotify();
}

Core::Items::DataPath* WsProjectManager::newGroup(Core::Items::DataPath *under, const QString &name, const QString &file_path){
    auto dom = config.createElement("group");
    dom.setAttribute("name", name);
    dom.setAttribute("file", file_path);

    auto node = new TreeItem(this, dom, false);
    static_cast<TreeItem*>(under)->addChild(node);

    return node;
}

Core::Items::DataPath *WsProjectManager::newItem(Core::Items::DataPath *group, const QString &file_path, const QString &name){
    auto dom = config.createElement("item");
    dom.setAttribute("name", name);
    dom.setAttribute("file", file_path);

    auto node = new TreeItem(this, dom);
    static_cast<TreeItem*>(group)->addChild(node);

    return node;
}

void WsProjectManager::move(Core::Items::DataPath *item, Core::Items::DataPath *group, int index){
    //== 实际类型
    auto item_real = static_cast<TreeItem*>(item);
    auto item_parent= static_cast<TreeItem*>(item->parent());
    //== child treeitem移除
    item_parent->removeChild(item_real);
    auto group_real = static_cast<TreeItem*>(group);
    group_real->addChild(item_real, index);
}

void WsProjectManager::remove(const Core::Items::DataPath *path){
    auto pnode = static_cast<TreeItem*>(path->parent());
    auto node2 = const_cast<Core::Items::DataPath*>(path);
    // dom remove====
    pnode->getNodeResp().removeChild(static_cast<TreeItem*>(node2)->getNodeResp());
    pnode->removeChild(static_cast<TreeItem*>(node2));
}

QVariant WsProjectManager::data(const Core::Items::DataPath *path, int role) const{

    switch (role) {
        case Qt::DecorationRole:{
                auto i = const_cast<Core::Items::DataPath*>(path);
                if(static_cast<TreeItem*>(i)->isLeaf())
                    return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
                else
                    return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
            }
        case Qt::DisplayRole:{
                return static_cast<TreeItem*>(const_cast<Core::Items::DataPath*>(path))
                        ->getNodeResp().attribute("name");
            }
        default:
            return QVariant();
    }
}
