#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include "../WsCore/include/datatype.h"

#include <QComboBox>
#include <QDialog>
#include <QTreeView>

class NewDialog : public QDialog
{
public:
    explicit NewDialog(Core::Group::Project *project, QWidget *parent=nullptr);
    virtual ~NewDialog();

private:
    Core::Group::Project *const project_base;
    QComboBox *const top_type;              // 筛选类别：[通用模板类型/增强插件生成]
    QComboBox *const file_type;             // 文件类型：[插件名称：类型名称(*.后缀名)]
    QTreeView *const project_struct;        // 显示项目列表及项目结构，选择指定节点
    QLineEdit *const name_input;            // 文件名，可以输入文件后缀名
    QPushButton *const ok_check, *const cancel_check;

private:
    void _toptype_selection(int index);
    void _check_ok_process();
    void _check_cancel_process();
};

#endif // NEWDIALOG_H
