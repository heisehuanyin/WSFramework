#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include "schedule.h"
#include "logwriter.h"
#include "configport.h"
#include "window.h"
#include "projectmanager.h"
#include "menumanager.h"

namespace Plugin{

    /**
     * @brief 插件管理器
     */
    class PluginsManager : public Base
    {
    public:
        virtual ~PluginsManager() = default;

        /**
         * @brief 加载指定文件夹内部所有插件动态库
         * @param plugins_home 插件文件夹
         */
        virtual void loadAllPlugins(const QString &plugins_home) = 0;

        /**
         * @brief 卸载指定插件实例以及关联下游插件实例
         * @param root 根插件
         */
        virtual void unloadRelatedPlugins(Base* root) = 0;

        /**
         * @brief 获取全部下游插件
         * @return
         */
        virtual QList<Base*> downstreamPlugins(Base *) = 0;

        /**
         * @brief 查询指定类型的已注册插件列表
         * @param type 类型
         * @return
         */
        virtual QList<Base*> queryRegisterdPlugins(PluginType type) = 0;

        // == plugin-query ====

        /**
         * @brief 获取已经加载的logwriter插件的一个实例, 后续需要调用者进行参数配置
         * @param host 调用此接口插件
         * @param registerName 插件注册名
         * @param logsdir log文件目录
         * @param core 核心插件
         */
        virtual LogWriter *newLogWriter(Base *host, const QString &registerName, const QString &logsdir, Plugin::Schedule *core) = 0;

        /**
         * @brief 获取已经加载的configport插件一个实例，后续调用者可以用来打开或新建配置文件
         * @param host 调用者
         * @param registerName 插件注册名
         * @param core 核心插件
         * @return
         */
        virtual ConfigPort *newConfigPort(Base *host, const QString &registerName,  Plugin::Schedule *core) = 0;

        virtual ProjectManager *newProjectManager(Base *host, const QString &regName, Core::Group::Visual *visual) = 0;

        // == UI =====
        virtual MenuManager *newMenuManager(Base *host, const QString &regName, Core::Group::Visual *visual) = 0;

        virtual Window *newWindow(Base *host, const QString &regName, Core::Group::Visual *visual) = 0;

    };


}

QT_BEGIN_NAMESPACE

#define PLUGINMANAGER "wsframework.plugin-set.pluginsmanager"
Q_DECLARE_INTERFACE(Plugin::PluginsManager, PLUGINMANAGER)

QT_END_NAMESPACE

#endif // PLUGINSMANAGER_H
