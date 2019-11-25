#ifndef FILEGENERATE_H
#define FILEGENERATE_H

#include "foundation.h"
#include "datatype.h"

namespace Plugin {
    class FileGenerate : public Base, public Configure
    {
    public:
        virtual ~FileGenerate() = default;

        /**
         * @brief 新建生成器实例，传入参数
         * @param project
         * @param xargs
         * @return
         */
        virtual FileGenerate *newGenerator(Core::Group::Project *project,
                                           QHash<QString,QString> xargs) = 0;

        virtual QString typeName() const = 0;
        virtual QString suffix() const = 0;
        /**
         * @brief 在指定文件夹路径下生成指定名称文件
         * @param dirPath
         * @param fileName
         */
        virtual void newFile(const QString &dirPath, const QString &fileName);
    };
}

#endif // FILEGENERATE_H
