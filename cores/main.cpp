/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "app_core.h"

__attribute__((constructor)) void load_file()
{
    printf("Constructor is called.\n");
}

__attribute__((destructor)) void unload_file()
{
    printf("destructor is called.\n");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    App_Core::bulid(p);

    return a.exec();
}
