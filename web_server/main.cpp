#include <QCoreApplication>
#include "web_core.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    Web_Core::bulid(p);

    return a.exec();
}
