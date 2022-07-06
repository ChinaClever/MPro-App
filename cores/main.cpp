#include <QCoreApplication>
#include "app_start.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(App_Run::isRun("cores")) a.exit();

    QObject *p = a.parent();
    App_Start::bulid(p);

    return a.exec();
}
