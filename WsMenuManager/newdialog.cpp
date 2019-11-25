#include "newdialog.h"
#include "../WsCore/include/schedule.h"
#include "../WsCore/include/foundation.h"
#include "../WsCore/include/filegenerate.h"

#include <QDir>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

NewDialog::NewDialog(Core::Group::Project *project, QWidget *parent)
    :QDialog (parent),project_base(project),top_type(new QComboBox(this)),
      file_type(new QComboBox(this)),project_struct(new QTreeView(this)),
      name_input(new QLineEdit(this)),ok_check(new QPushButton("确定", this)),
      cancel_check(new QPushButton("取消",this))
{
    auto layout = new QGridLayout(this);
    layout->setMargin(1);
    layout->addWidget(top_type);
    layout->addWidget(file_type, 0, 1, 1, 3);
    layout->addWidget(project_struct, 1, 0, 4, 4);
    layout->addWidget(name_input, 5, 0, 1, 4);
    layout->addWidget(cancel_check, 6, 0, 1, 2);
    layout->addWidget(ok_check, 6, 2, 1, 2);

    connect(top_type,       QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,           &NewDialog::_toptype_selection);
    connect(ok_check,       &QPushButton::clicked,
            this,           &NewDialog::_check_ok_process);
    connect(cancel_check,   &QPushButton::clicked,
            this,           &NewDialog::_check_cancel_process);

    top_type->addItems(QStringList() << "Generic" << "Custom");
    top_type->setCurrentIndex(0);
    project_struct->setModel(project_base->visualSet()->getProjectsTree());
}

NewDialog::~NewDialog()
{

}

void NewDialog::_toptype_selection(int index)
{
    switch (index) {
        case 0:{
                auto visual = project_base->visualSet();
                auto tdir = visual->schedule()->getTemplatesPath();
                QDir templates(tdir);
                auto flist = templates.entryInfoList(QDir::Files);
                file_type->clear();
                for (auto f : flist) {
                    auto item = f.baseName()+"(*."+f.suffix()+")";
                    file_type->addItem(item, f.fileName());
                }
            }break;
        case 1:{
                auto allg = project_base->allGenerators();
                file_type->clear();
                for (auto g : allg) {
                    auto item = g->registerName()+":"+g->typeName()+"(*."+g->suffix()+")";
                    file_type->addItem(item, g->registerName());
                }
            }break;
    }
}

void NewDialog::_check_ok_process()
{
    // select top-type
    // select file-type
    // select project-struce
    // enter file-name
    this->close();
}

void NewDialog::_check_cancel_process()
{
    this->close();
}
