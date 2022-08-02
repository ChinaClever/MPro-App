/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "app_start.h"
#include "sercret_sm.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    App_Start::bulid(p);


    Sercret_SM::sm4Cfg.key = "1234567890123456";
    Sercret_SM::sm4Cfg.iv = "asdfghjklzxcvbnm";
//    s.setKey("1234567890123456");
//    s.setIv("asdfghjklzxcvbnm");

    Sercret_SM *sm = new Sercret_SM();
    QByteArray res = sm->sm4_encrypt("luozhiyong");
    qDebug() << sm->sm4_decrypt(res);

    return a.exec();
}
