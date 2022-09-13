/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_com.h"

Cfg_Com::Cfg_Com(const QString &fn, QObject *parent)
    : Cfg_Obj(fn, parent)
{

}

void Cfg_Com::initCfg()
{
    QCoreApplication::setOrganizationName("CLEVER");
    QCoreApplication::setOrganizationDomain("clever.com");
    QCoreApplication::setApplicationName("PDU-Pro");
}

Cfg_Com *Cfg_Com::bulid(QObject *parent, const QString& fn)
{
    static Cfg_Com* sington = nullptr;
    if(!sington) {
        initCfg(); QString name = pathOfCfg(fn);
        sington = new Cfg_Com(name, parent);
    }
    return sington;
}

