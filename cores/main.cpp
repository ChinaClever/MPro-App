#include <QCoreApplication>
#include "app_start.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    App_Start::bulid(p);

    return a.exec();
}
