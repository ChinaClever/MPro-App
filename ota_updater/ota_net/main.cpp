/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "ota_core.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    Ota_Core::bulid(p);

    return a.exec();
}
