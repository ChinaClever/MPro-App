/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_net.h"

Ota_Net::Ota_Net(QObject *parent)
    : Ota_Obj{parent}
{    
    qRegisterMetaType<sOtaFile>("sOtaFile");
    mDtls = Dtls_Recver::bulid(this); mDtls->listen();
    connect(mDtls, &Dtls_Recver::startSig, this, &Ota_Net::startSlot);
    connect(mDtls, &Dtls_Recver::finishSig, this, &Ota_Net::finishSlot);
    mOta = &(cm::dataPacket()->ota);
}

void Ota_Net::startSlot(const QString &host)
{
    sOtaUpdater *ota = mOta;
    qstrcpy(ota->host, host.toUtf8().data());
    ota->isRun = 2; ota->isOk = 1;
}

QString Ota_Net::unzip(const QString &fn)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    QString dst = "/usr/data/updater/";
#else
    QString dst = "/home/lzy/work/updater/";
#endif
    QString str = "unzip -o %1 -d " + dst;
    str = cm::execute(str.arg(fn));
    throwMessage(str);
    return dst;
}

bool Ota_Net::coreRuning()
{
    QString cmd = "/usr/data/clever/app/proc_run cores";
    return cm::execute(cmd).toInt();
}

void Ota_Net::cmd_updater()
{
    QString cmd = "/usr/data/clever/app/pdu_cmd";
    cmd += " pduCfgSet 83 11 1";
    system(cmd.toUtf8().data());
}

void Ota_Net::workDown()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    bool ret = coreRuning();
    if(ret) {
        cmd_updater();
    } else {
        system("reboot");
    }
#endif
}

void Ota_Net::finishSlot(const sOtaFile &it, bool ok)
{
    sOtaUpdater *ota = mOta; if(ok) {
        ota->isRun = 2; ota->isOk = 0;
        QString dir = unzip(it.path+it.file);
        if(QFile::exists(dir+"auto.sh")) {
            QString str = "sh %1/auto.sh ";
            str = cm::execute(str.arg(dir));
            throwMessage(str);
        } else workDown();
    } else {
        ota->isOk = 2;
        QString fn = it.path + it.file;
        QString cmd = "rm -f " + fn;
        system(cmd.toUtf8().data());
    }
}
