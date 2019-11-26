#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <QObject>
#include <QAction>

namespace Plugin{
    // enum-int
    enum PluginType{
        LUNCH_SCHEDULE,         // 模式启动器[图形、静默]
        PLUGINSMANAGER,         // 核心:插件管理器
        LOGWRITER,              // Log输出
        CONFIGPORT,             // 配置端口
        INDEPENDENTTOOLS,       // 独立功能插件
        PROJECTMANAGER,         // 项目管理器
        PROJECTENHANCE,         // 项目增强插件
        PROJECTFILEGEN,         // 文件生成器

        BINARYPRESENT,          // 二进制处理模块
        TEXTPRESENT,            // 文字处理模块
        ITEMSPRESENT,           // 条目数据提供者

        GRAPHICSFRONTEND,       // 窗口框架前端
        MENUSMANAGER,           // 菜单管理器
        ITEMSVIEW,              // 条目视图
        TEXTVIEW,               // 普通文本显示
    };

    /**
     * @brief 所有插件共同基类
     */
    class Base
    {
    public:
        virtual ~Base() = default;

        virtual QString registerName() const = 0;
        virtual PluginType pluginType() const = 0;

        virtual void save() = 0;
    };

    /**
     * @brief 插件具有可结合特性
     */
    class Combine
    {
    public:
        virtual ~Combine() = default;

        /**
         * @brief 预定义所需插件
         * @return 插件类型
         */
        virtual QList<PluginType> pluginsRequired() const = 0;
    };

    /**
     * @brief 插件具有可配置特性
     */
    class Configure
    {
    public:
        enum ArgsType{
            STRING_INPUT,
            NUMBER_INPUT,
            KEYWORDS_ENUM
        };

        virtual ~Configure() = default;

        /**
         * @brief 重新配置插件，改变插件行为
         * @param xargs
         */
        virtual void argsConfigure(const QHash<QString,QString> &xargs) = 0;
        virtual QList<QString> configItems() const = 0;
        virtual QString argumentValue(const QString &value) const = 0;
        virtual QPair<ArgsType,QList<QString>> argumentInfo(const QString &name) const = 0;
    };
}

QT_BEGIN_NAMESPACE

#define PLUGIN_BASE "wsframework.plugin-set.pluginbase"
Q_DECLARE_INTERFACE(Plugin::Base, PLUGIN_BASE)

#define PLUGIN_CONBINE "wsframework.plugin-set.conbine"
Q_DECLARE_INTERFACE(Plugin::Combine, PLUGIN_CONBINE)

#define PLUGIN_CONFIGURE "wsframework.plugin-set.configure"
Q_DECLARE_INTERFACE(Plugin::Configure, PLUGIN_CONFIGURE)

QT_END_NAMESPACE

#endif // FOUNDATION_H
