#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include "foundation.h"
#include "datatype.h"

namespace Plugin {
    class FileGenerator : public Base, public Configure
    {
    public:
        virtual ~FileGenerator() = default;

        /**
         * @brief 新建生成器实例，传入参数
         * @param project
         * @param xargs
         * @return
         */
        virtual FileGenerator *newGenerator(Core::Group::Project *project) = 0;

        /**
         * @brief 类型名称
         * @return
         */
        virtual QString typeName() const = 0;
        /**
         * @brief 文件类型后缀名
         * @return
         */
        virtual QList<QString> suffixes() const = 0;
        /**
         * @brief 在指定文件夹路径下生成指定名称文件
         * @param dirPath
         * @param baseName
         */
        virtual void fileGenerate(const QString &dirPath, const QString &baseName);
    };
}

QT_BEGIN_NAMESPACE

#define PLUGIN_FILEGENERATOR "wsframework.plugin-set.filegenerator"
Q_DECLARE_INTERFACE(Plugin::FileGenerator, PLUGIN_FILEGENERATOR)

QT_END_NAMESPACE

#endif // FILEGENERATOR_H
