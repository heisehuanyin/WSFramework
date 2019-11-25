#ifndef ITEMSPRESENT_H
#define ITEMSPRESENT_H

#include "datatype.h"

namespace Plugin {
    class ItemsPresent
    {
    public:
        virtual ~ItemsPresent() = default;

        /**
         * @brief 获取根节点路径
         * @return
         */
        virtual Core::Items::DataPath *root() = 0;

        /**
         * @brief 获取数据header
         * 横向X轴优先
         * root
         *  |——横(-,0)
         *  |   |--横1(1,0)
         *  |   |--横2(2,0)
         *  |   |--横3(3,0)
         *  |   ...
         *  |   |--横n(n,0)
         *  |
         *  |——纵(0,-)
         *      |--纵1(0,1)
         *      |--纵2(0,2)
         *      |--纵3(0,3)
         *      ...
         *      |--纵n(0,n)
         *
         * @param role
         * @return
         */
        virtual QVariant header(Core::Items::DataPath *node, Qt::ItemDataRole role) = 0;

        /**
         * @brief data
         * @param node
         * @param role
         * @return
         */
        virtual QVariant data(Core::Items::DataPath *node, Qt::ItemDataRole role) = 0;

        virtual void setData(Core::Items::DataPath *node, Qt::ItemDataRole role, QVariant data) = 0;
    };
}

#endif // ITEMSPRESENT_H
