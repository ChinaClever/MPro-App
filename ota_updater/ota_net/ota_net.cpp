/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_net.h"
#include "cfg_app.h"

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
    sOtaUpdater *ota = mOta; setbit(ota->work, 2);
    qstrcpy(ota->host, host.toUtf8().data());
    ota->net.isRun = 1;
}

QString Ota_Net::unzip(const QString &fn)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    QString dst = "/usr/data/";
    system("mkdir -p /usr/data/updater/clever/");
#else
    QString dst = "/home/lzy/work/";
#endif    
    QString str = "unzip -o %1 -d " + dst+"updater/clever/";
    throwMessage(str.arg(fn)); str = cm::execute(str.arg(fn));
    throwMessage(str); system("rm -rf /usr/data/clever/upload/*");
    return dst+"updater/clever/";
}

bool Ota_Net::coreRuning()
{
    QString cmd = "proc_run cores";
    return cm::execute(cmd).toInt();
}

bool Ota_Net::cmd_updater(const QString &fn)
{
    QString cmd = "pdu_cmd";
    cmd += " pduCfgSet 83 11 " + fn;
    bool ret = cm::execute(cmd).toInt();
    cmd += " res:" + QString::number(ret);
    throwMessage(cmd);
    return ret;
}

void Ota_Net::workDown(const QString &fn)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    bool ret = coreRuning();
    if(ret) ret = cmd_updater(fn);
    if(!ret) {
        clrbit(mOta->work, 2); if(!mOta->work)
        QTimer::singleShot(3555,this,SLOT(rebootSlot()));
    }
#endif
}

void Ota_Net::rebootSlot()
{
    QString cmd = "cp -af /usr/data/updater/clever/  /usr/data/";
    throwMessage(cmd); throwMessage(cm::execute(cmd));
    system("chmod +x /usr/data/clever/bin/*");
    system("chmod +x /usr/data/clever/app/*");
    cmd = "rm -rf /usr/data/updater/clever";
    throwMessage(cmd); throwMessage(cm::execute(cmd));

    throwMessage("start now reboot"); cm::mdelay(1);
    cm::execute("rm -rf /usr/data/clever/upload/*");
    cm::execute("sync"); system("reboot");
}

bool Ota_Net::versionCheck(const QString &dir)
{
    sAppVerIt it;
    Cfg_App app(dir, this);
    bool ret = app.app_unpack(it);
    throwMessage(dir); if(ret) {
        throwMessage("version check ok");
        QString str = cm::masterDev()->cfg.vers.releaseDate;
        if(str.size()) {
            QDate ct = QDate::fromString(str, "yyyy-MM-dd");
            QDate date = QDate::fromString(it.releaseDate, "yyyy-MM-dd");
            throwMessage("version release date " +it.releaseDate); if(date < ct) {
                QString msg = "version release date err: currnet date %1 up date:%2";
                throwMessage(msg.arg(str, it.releaseDate)); ret = false;
            } else {
                throwMessage("version dev type "+it.dev);
                str = cm::masterDev()->cfg.vers.dev;if(str.size()) {
                    if(str != it.dev) {
                        QString msg = "version dev type err: currnet type %1 up type:%2";
                        throwMessage(msg.arg(str, it.dev)); ret = false;
                    }
                }
            }
        }
    } else {
        QString msg = "version check err %1 %2 %3 %4";
        throwMessage(msg.arg(it.usr, it.dev, it.releaseDate, it.md5));
    }

    return ret;
}

void Ota_Net::finishSlot(const sOtaFile &it, bool ok)
{
    QString dir; if(ok) {
        dir = unzip(it.path+it.file);
        ok = versionCheck(dir);
    }

    sOtaUpdater *ota = mOta; if(ok) {
        if(QFile::exists(dir+"auto.sh")) {
            QString str = "sh %1/auto.sh ";
            str = cm::execute(str.arg(dir));
            throwMessage(str);
        } else workDown(it.path+it.file);
        ota->net.isRun = 0;
    } else {
        ota->net.isRun = 2;
        QString fn = it.path + it.file;
        QString cmd = "rm -f " + fn;
        system(cmd.toUtf8().data());
    } clrbit(ota->work, 2);
}
