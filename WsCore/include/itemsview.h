#ifndef ITEMSVIEW_H
#define ITEMSVIEW_H

#include "foundation.h"
#include "datatype.h"

#include <QAbstractItemModel>

namespace Plugin {

    class ItemsView : public Base, public Combine
    {
    public:
        virtual ~ItemsView() = default;

        /**
         * @brief 新建treeview实例
         * @param file 文件通道，若nullptr建立空白视图
         * @return
         */
        virtual ItemsView *newItemsView(Core::Group::Visual *view, Core::Group::File *file = nullptr) = 0;

        /**
         * @brief 重置视图模型
         * @param model
         */
        virtual void resetModel(QAbstractItemModel *model) = 0;

        /**
         * @brief getWidget
         * @return
         */
        virtual QWidget *getWidget() = 0;

        virtual bool visible() = 0;

        virtual void setVisible(bool v) = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGIN_ITEMSVIEW "wsframework.plugin-set.itemsview"
Q_DECLARE_INTERFACE(Plugin::ItemsView, PLUGIN_ITEMSVIEW)

QT_END_NAMESPACE

#endif // ITEMSVIEW_H
