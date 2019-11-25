#ifndef DATATYPE_H
#define DATATYPE_H

#include <QAbstractItemModel>
#include <QIcon>
#include <QString>

namespace Plugin {
    class Base;
    class ProjectManager;
    class LogWriter;
    class ConfigPort;
    class Schedule;
    class Window;
    class MenuManager;
    class Independent;
    class EnhancePlugin;
}

namespace Core {
    namespace Items {
        class DataPath;

        class DataPathListener{
        public:
            virtual ~DataPathListener() = default;
            /**
             * @brief 节点数据变更
             * @param item 指定节点
             */
            virtual void dataChanged(DataPath* item) = 0;
            /**
             * @brief 插入新节点到集合节点
             * @param group 目标集合节点
             * @param row 新集合中row-index
             * @param column 新集合中column-index
             */
            virtual void nodeInsert(DataPath* group, int row, int column) = 0;
            /**
             * @brief 移除既有节点
             * @param group 包含集合节点
             * @param row 原集合中row-index
             * @param column 原集合中column-index
             */
            virtual void nodeRemove(DataPath* group, int row, int column) = 0;
        };

        /**
         * @brief 为了方便多项目集成构建此类型，path中包含的所有指针自行管理
         */
        class DataPath
        {
        public:
            virtual ~DataPath() = default;
            /**
             * @brief 接受ChangeListener，数据变更时调用，不管理listener
             * @param listener 监听器
             */
            virtual void registerListener(DataPathListener *listener) = 0;
            virtual bool containsListener(DataPathListener *listener) const = 0;
            /**
             * @brief path关联模型，返回一个 void* 作为辨识用
             * @return
             */
            virtual void *model() const = 0;
            /**
             * @brief root节点的父节点为nullptr
             * @return
             */
            virtual DataPath* parent() const = 0;
            virtual int row() = 0;
            virtual int column() = 0;
            virtual DataPath* childAt(int row, int column = 0) const= 0;
            virtual int rowCount() const = 0;
            virtual int columnCount() const = 0;
            virtual bool isLeaf() const = 0;
            virtual int indexOf(DataPath* child) const = 0;
            /**
             * @brief 相关数据
             * @param type 数据类别
             * @return
             */
            virtual QVariant data(int type) const = 0;
        };
    }

    namespace Group {
        class Project;

        class File
        {
        public:
            virtual ~File() = default;

            virtual Project* projectGroup() const = 0;

            /**
             * @brief 文件打开，加载系列插件
             */
            virtual void openTarget() = 0;
            /**
             * @brief 文件保存，犀利插件操作
             */
            virtual void save() = 0;
            /**
             * @brief 指向文件路径
             * @return
             */
            virtual QString filePath() const = 0;
            /**
             * @brief 获取单纯文件名称
             * @return
             */
            virtual QString fileName() const = 0;
            /**
             * @brief 获取通道操作接口
             * @return
             */
            virtual QList<Plugin::Base*> head() const = 0;
            /**
             * @brief 通道是否包含指定插件
             * @param target
             * @return
             */
            virtual bool contains(Plugin::Base* target) const = 0;

            /**
             * @brief 获取所有插件关联的schedule实例
             * @return
             */
            virtual Plugin::Schedule *schedule() const = 0;

            /**
             * @brief 获取指定插件的上游插件
             * @param target
             * @return
             */
            virtual QList<Plugin::Base*> upStream(Plugin::Base *target) const = 0;

            /**
             * @brief 获取指定插件的下游插件，由配置文件指定，可能未生成，需要立刻生成
             * @param target
             * @return
             */
            virtual Plugin::Base *downStream(Plugin::Base *target) = 0;
        };

        class Visual;
        /**
         * @brief The ProjectGroup class
         * 项目管理上下文
         * 1. 项目管理器
         * 2. 项目log端口
         * 3. 项目配置端口
         * 4. 项目功能增强插件
         */
        class Project
        {
        public:
            virtual ~Project() = default;

            virtual Visual* visualSet() const = 0;
            virtual void openTarget(const QString &project_file) = 0;
            virtual void newTarget(const QString &pluginName, const QString &project_root_path) = 0;
            virtual void saveTarget() = 0;
            /**
             * @brief 获取所有插件关联的schedule实例
             * @return
             */
            virtual Plugin::Schedule *schedule() const = 0;
            virtual bool contains(Plugin::Base *target) = 0;
            virtual Plugin::ProjectManager *projectManager() const = 0;
            virtual Plugin::LogWriter *logWriter() const = 0;
            virtual Plugin::ConfigPort *configPort() const = 0;
            virtual QList<Plugin::Base*> getEnhancePlugins() const = 0;

            virtual QString projectRootPath() const = 0;
            /**
             * @brief 打开文件，返回实例(打开)，新建操作采用模板文件不提供新建接口
             * @param filePath
             */
            virtual File *openFile(const QString &filePath) = 0;
            virtual QList<QString> openedFiles() const = 0;
            /**
             * @brief 获取文件通道，若未打开返回nullptr
             * @param filePath
             * @return
             */
            virtual File *getFile(const QString &filePath) const = 0;

            virtual void saveFile(File *file) = 0;
            virtual void closeFile(File *file) = 0;
        };

        /**
         * @brief The VisualSet class
         * 代表一个可视化图形界面，包含：
         * 1. 窗口框架
         * 2. 菜单管理器
         * 3. 内容显示视图列表
         * 4. 项目管理器列表
         * 5. 项目管理树
         */
        class Visual
        {
        public:
            virtual ~Visual() = default;

            /**
             * @brief 获取所有插件关联的schedule实例
             * @return
             */
            virtual Plugin::Schedule *schedule() const = 0;
            virtual Plugin::MenuManager *getMenuManager () const = 0;
            /**
             * @brief 获取窗口指针，如果窗口未打开显示，返回nullptr
             * @return
             */
            virtual Plugin::Window *getWindow() const = 0;
            virtual bool contains(Plugin::Base *target) = 0;
            virtual QList<Plugin::Base*> getIndependentTools() const = 0;

            /**
             * @brief 打开项目，何种项目管理器通过全局配置判定
             * @return
             */
            virtual Project *openProject(const QString &project_file) = 0;

            /**
             * @brief 打开指定项目
             * @param plugin 项目管理器注册名
             * @param project_dir 项目文件夹路径
             * @return
             */
            virtual Project *newProject(const QString &plugin, const QString &project_dir) = 0;

            virtual void saveProject(Project *pjt) = 0;
            virtual void closeProject(Project *pjt) = 0;

            /**
             * @brief 活跃项目路径集合
             * @return
             */
            virtual QList<QString> aliveProjects() const = 0;

            /**
             * @brief 获取处于唯一激活位置的项目
             * @return 项目路径
             */
            virtual QString activedProject() const = 0;

            /**
             * @brief 通过项目路径，获取关联活跃项目
             * @param project_dir
             * @return
             */
            virtual Project *getProject(const QString &project_dir) const = 0;

            /**
             * @brief 激活指定项目
             * @param project
             */
            virtual void activeProject(Project *project) = 0;

            /**
             * @brief 获取界面中所有项目组成的树形结构表达
             * @return
             */
            virtual QAbstractItemModel *getProjectsTree() const = 0;

            /**
             * @brief 通过projects-tree中的index获取实际项目描述节点
             * @param index 索引
             * @return
             */
            virtual Core::Items::DataPath *nodeConvert(const QModelIndex &index) const = 0;

            /**
             * @brief 刷新可见菜单
             */
            virtual void refreshVisibleMenus() = 0;
        };
    }
}

#endif // DATATYPE_H
