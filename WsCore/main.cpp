#include "wscore.h"

#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WsCore luncher;
    auto nvisual = luncher.openWindow();
    luncher.openDefaultProject(nvisual);

    qDebug() << "result.";

    return a.exec();
}
