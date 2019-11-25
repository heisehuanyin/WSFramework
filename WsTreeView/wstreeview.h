#ifndef WSTREEVIEW_H
#define WSTREEVIEW_H

#include "wstreeview_global.h"
#include "../WsCore/include/itemsview.h"
#include "../WsCore/include/window.h"

#include <QTreeView>

class WSTREEVIEWSHARED_EXPORT WsTreeView
        :public QObject, public Plugin::ItemsView
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_ITEMSVIEW)
    Q_INTERFACES(Plugin::ItemsView Plugin::Base)

public:
    WsTreeView(){}


    // Combine interface
public:
    virtual QList<Plugin::PluginType> pluginsRequired() const override{
        return QList<Plugin::PluginType>();
    }

    // Base interface
public:
    virtual QString registerName() const override{
        return "WsTreeView";
    }
    virtual Plugin::PluginType pluginType() const override{
        return Plugin::ITEMSVIEW;
    }
    virtual void save() override{

    }

    // ItemsView interface
public:
    virtual Plugin::ItemsView *newItemsView(Core::Group::Visual *view, Core::Group::File *file) override{
        return nullptr;
    }
    virtual void resetModel(QAbstractItemModel *model) override{

    }
    virtual QWidget *getWidget() override{
        return nullptr;
    }
    virtual bool visible() override{
        return false;
    }
    virtual void setVisible(bool v) override{

    }
};

#endif // WSTREEVIEW_H
