#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "datatype.h"
#include "schedule.h"

namespace Plugin {
    /**
     * @brief The ProjectManager class
     * 项目管理器：
     * 1.提供项目和项目内条目管理，
     * 2.提供简单的key-value配置功能
     */
    class ProjectManager : public Base, public Configure
    {
    public:
        virtual ~ProjectManager() = default;

        /**
         * @brief 获取配置
         * @param key
         * @return
         */
        virtual QString getValue(const QString &key) = 0;
        /**
         * @brief 设置配置
         * @param key
         * @param val
         */
        virtual void setValue(const QString &key, const QString &val) = 0;

        /**
         * @brief 新建实例
         * @param core
         * @return
         */
        virtual ProjectManager* newManager(Core::Group::Visual* visual) = 0;

        virtual void openProject(const QString &project_file) = 0;
        virtual void newProject(const QString &project_root_path) = 0;

        /**
         * @brief 重命名节点名称
         * @param path 节点路径
         * @param name 名称
         */
        virtual void rename(const Core::Items::DataPath *path, const QString &name) = 0;

        /**
         * @brief 获取project项目根目录
         * @return
         */
        virtual QString projectRootPath() const = 0;

        /**
         * @brief 获取节点路径对应的文件路径
         * @param path 节点路径
         * @return 文件系统路径
         */
        virtual QString getFilePath(const Core::Items::DataPath *path) const = 0;

        /**
         * @brief 返回本模型的根节点，父节点为nullptr
         * @return
         */
        virtual Core::Items::DataPath* description() const = 0;

        /**
         * @brief 新建集合节点
         * @param under 包含集合
         * @param name 新集合名称
         * @param file_path 可选文件路径
         */
        virtual Core::Items::DataPath* newGroup(Core::Items::DataPath *under, const QString &name = "新建集合", const QString &file_path = QString()) = 0;

        /**
         * @brief 新叶子节点
         * @param group     包含节点
         * @param file_path 文件路径
         * @param name      节点名称
         */
        virtual Core::Items::DataPath* newItem(Core::Items::DataPath *group, const QString &file_path, const QString &name = "新建节点") = 0;

        /**
         * @brief 指定节点移动，默认添加到末尾
         * @param item 移动节点
         * @param group 目标位置集合
         * @param index group中index
         */
        virtual void move(Core::Items::DataPath *item, Core::Items::DataPath *group, int index = -1) = 0;

        /**
         * @brief 删除节点
         * @param path 节点路径
         */
        virtual void remove(const Core::Items::DataPath *path) = 0;

        /**
         * @brief 方便和Qt集成，采用了Qt节点数据分类
         * @param path path节点
         * @param role 数据
         * @return
         */
        virtual QVariant data(const Core::Items::DataPath *path, int role) const = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGINPROJECTMANAGER "wsframework.plugin-set.projectmanager"
Q_DECLARE_INTERFACE(Plugin::ProjectManager, PLUGINPROJECTMANAGER)

QT_END_NAMESPACE

#endif // PROJECTMANAGER_H
