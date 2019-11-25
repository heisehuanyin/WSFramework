#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "schedule.h"

#include <QMenu>

namespace Plugin {
    /**
     * @brief The MenuManager class
     *
     * 1. 文件操作
     *      * 新建文件：{文件模板1，文件模板2...文件模板n}
     *      * 打开文件
     *      * 关闭文件：{打开文件1，打开文件2...打开文件n}
     *      * 文件历史：{历史文件1，历史文件2...历史文件n}
     *      ----------------------------------------
     *      * 新建项目：{类别1、类别2...类别n}
     *      * 打开项目
     *      * 关闭项目：{打开项目1，打开项目2...打开项目n}
     *      * 历史项目：{历史项目1，历史项目2...历史项目n}
     *      ----------------------------------------
     *      * 打印
     *      ----------------------------------------
     *      * 退出软件
     *
     * 2. 视图编辑操作：由当前视图提供/以下示例
     *      * 撤销操作
     *      * 重做操作
     *      ----------------------------------------
     *      * 全选
     *      * 剪切
     *      * 复制
     *      * 粘贴
     *      * 编辑历史
     *      ----------------------------------------
     *      * 查找
     *      * 替换
     *      * 转到行
     *
     * 3. 项目功能增强插件
     *      * 编译相关
     *      * 调试相关
     *      * 分析相关
     *      * ...
     *
     * 4. 独立工具
     *      * 独立工具1
     *      * 独立工具2
     *      * ...
     *      * 独立工具n
     *
     * 5. 当前激活插件
     *      * 插件1-menu
     *      * 插件2-menu
     *      * ...
     *      * 插件n-menu
     *
     * 6. 视图和窗口
     *      * 全屏操作
     *      * 最小化当前窗口
     *      ----------------------------------------
     *      * 窗口组1
     *      * 窗口组2
     *      * ...
     *      * 窗口组n
     *      ----------------------------------------
     *      * 视图-1
     *      * 视图-2
     *      * ......
     *      * 视图-n
     *
     * 7. 帮助
     *      * 插件管理
     *      * 全局设置
     *      ----------------------------------------
     *      * 帮助信息
     *      * 系统信息
     *      ----------------------------------------
     *      * 软件激活
     *      * 关于软件
     */
    class MenuManager : public Base
    {
    public:
        virtual ~MenuManager() = default;

        /**
         * @brief 获取新menu-manager
         * @param core 核心
         * @param win 绑定窗口
         * @return 新管理器
         */
        virtual MenuManager* newMenuManager(Core::Group::Visual *g) = 0;

        /**
         * @brief 菜单组数量
         * @return
         */
        virtual int groupCount() const = 0;
        /**
         * @brief 获取菜单组项目
         * @param index 索引
         * @return
         */
        virtual QMenu *menuGroup(int index) = 0;

        /**
         * @brief 在指定位置注册菜单项
         * @param pos 预定义位置
         * @param actions 菜单项
         * @param host 注册行为发起插件
         */
        virtual void registerAction(QAction* actions, Base *host) = 0;

        /**
         * @brief 输入当前界面绑定插件，激活所有关联插件
         * @param actived
         */
        virtual void menuActive(const QList<Base*> &actived) = 0;

        /**
         * @brief 移除指定插件相关菜单
         * @param plugin
         */
        virtual void removeRelatedMenus(Plugin::Base* plugin) = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGINMENUMANAGER "wsframework.plugin-set.menumanager"
Q_DECLARE_INTERFACE(Plugin::MenuManager, PLUGINMENUMANAGER)

QT_END_NAMESPACE

#endif // MENUMANAGER_H
