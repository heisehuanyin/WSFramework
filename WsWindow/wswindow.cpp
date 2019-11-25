#include "wswindow.h"
#include "../WsCore/include/projectmanager.h"

#include <QDir>
#include <QDockWidget>
#include <QStackedLayout>



WsWindow::WsWindow()
    :WsWindow(nullptr)
{
}

WsWindow::WsWindow(Core::Group::Visual *visual)
    :visual(visual),window(new QMainWindow(nullptr)),
      project_view(new QTreeView(window)),main(new QTabWidget(window))
{
    auto dock = new QDockWidget("项目管理", window);

    dock->setWidget(project_view);
    project_view->setHeaderHidden(true);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    window->addDockWidget(Qt::LeftDockWidgetArea, dock);
    project_view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(project_view,   &QTreeView::customContextMenuRequested,
            this,           &WsWindow::showProjectManagerMenu);

    window->setCentralWidget(main);
}

WsWindow::~WsWindow()
{
    delete window;
}

QString WsWindow::registerName() const{
    return "WsWindow";
}

Plugin::PluginType WsWindow::pluginType() const
{
    return Plugin::GRAPHICSFRONTEND;
}


void WsWindow::save(){

}

Plugin::Window *WsWindow::newWindow(Core::Group::Visual *visual)
{
    auto x = new WsWindow(visual);
    x->project_view->setModel(visual->getProjectsTree());

    return x;
}

void WsWindow::show(){
    window->show();
}

void WsWindow::resize(int width, int height){
    window->resize(width, height);
}

void WsWindow::loadMenuManager(Plugin::MenuManager *menus){
    auto mbar = window->menuBar();

    for (int index=0; index < menus->groupCount(); ++index) {
        mbar->addMenu(menus->menuGroup(index));
    }
}

QWidget *WsWindow::getWidget(){
    return window;
}

void WsWindow::showProjectManagerMenu(const QPoint &point)
{
    auto gpoint = project_view->mapToGlobal(point);
    auto index = project_view->indexAt(point);
    auto dpath = this->visual->nodeConvert(index);
    project_view->setCurrentIndex(index);

    QMenu contex("项目管理", project_view);

    auto ngroup = contex.addAction("新建集合", this, &WsWindow::newGroupNode);
    ngroup->setDisabled(dpath->isLeaf());

    auto m = contex.addMenu("新建文件");
    QDir temp(visual->schedule()->getTemplatesPath());
    auto list = temp.entryInfoList(QDir::Files);
    for (auto item : list) { m->addAction(item.fileName()); }
    connect(m,  &QMenu::triggered,  this,   &WsWindow::newItemNode);

    contex.addAction("删除节点及文件", this, &WsWindow::removeTargetFile);

    contex.addSeparator();

    contex.addAction("添加文件引用",  this,   &WsWindow::appendItemNode);

    contex.addAction("删除引用", this, &WsWindow::removeReference);

    contex.exec(gpoint);
}

void WsWindow::newGroupNode()
{
    auto index = project_view->currentIndex();
    auto path = visual->nodeConvert(index);
    static_cast<Plugin::ProjectManager*>(path->model())->newGroup(path);
}

void WsWindow::newItemNode(QAction *item)
{

}

void WsWindow::appendItemNode()
{

}

void WsWindow::removeTargetFile()
{

}

void WsWindow::removeReference()
{

}
