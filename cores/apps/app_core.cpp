/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_core.h"
#include "cfg_core.h"
#include "cfg_app.h"

App_Core::App_Core(QObject *parent)
    : App_WhiteList{parent}
{
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    Shm::initShm();
#else
    initVer();
#endif
    Cfg_Core::bulid();
}

App_Core *App_Core::bulid(QObject *parent)
{
    static App_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new App_Core(parent);
    }
    return sington;
}

void App_Core::compileTime()
{
    cm::dataPacket()->mem_size = sizeof(sDataPacket);
    sVersions *vers = &(cm::masterDev()->cfg.vers);
    cm::buildDateTime(vers->compileDate);
}

void App_Core::initUuid()
{
    QString fn = "/usr/data/clever/cfg/uuid.conf";
    if(!QFile::exists(fn)) {
        if(QDate::currentDate().year() < 2023) return;
        QString uuid = QUuid::createUuid().toString();
        QString cmd = "echo '%1' > %2";
        cmd = cmd.arg(uuid, fn); qDebug() << cmd;
        system(cmd.toStdString().c_str());
    }  QString cmd = "cat %1";
    QString res = cm::execute(cmd.arg(fn)).remove("\n");
    res = res.remove("{").remove("}");
    char *ptr = cm::masterDev()->cfg.uut.uuid;
    qstrcpy(ptr, res.toLocal8Bit().data());
    qDebug() << res;
}

QString App_Core::hashPassword(const QString& password)
{
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return QString::fromUtf8(hash.toHex()).right(8);
}

void App_Core::initRoot(const QString &sn)
{
    QString fmd = "echo -e '%1\n%1' |  passwd root";
    QString pwd = "123456"; if(sn.size() > 3) pwd = hashPassword(sn);
    QString cmd = fmd.arg(pwd); system(cmd.toStdString().c_str());
    char *ptr = cm::dataPacket()->login[0].reserve[3];
    qstrcpy(ptr, pwd.toStdString().c_str());
}

void App_Core::initVer()
{
    sVersions *ver = &(cm::masterDev()->cfg.vers);
    sAppVerIt it; Cfg_App cfg("/usr/data/clever/");
    bool ret = cfg.app_unpack(it); if(ret) {
        cm::buildDateTime(ver->compileDate);
        qstrcpy(ver->ver, it.ver.toUtf8().data());
        qstrcpy(ver->md5, it.md5.toUtf8().data());
        qstrcpy(ver->usr, it.usr.toUtf8().data());
        qstrcpy(ver->dev, it.dev.toUtf8().data());
        qstrcpy(ver->hwVersion, it.hw.toUtf8().data());
        qstrcpy(ver->serialNumber, it.sn.toUtf8().data());
        qstrcpy(ver->oldVersion, it.oldVersion.toUtf8().data());
        qstrcpy(ver->releaseDate, it.releaseDate.toUtf8().data());
        qstrcpy(ver->upgradeDate, it.upgradeDate.toUtf8().data());
        qstrncpy(ver->remark, it.remark.toUtf8().data(), sizeof(ver->remark));
        //cout << sizeof(ver->remark) << it.remark << ver->remark;
    } else {ver->md5[0] = 0; cout << CFG_APP << "error";}
    QString fn = "/usr/data/clever/cfg/sn.conf";
    if(it.sn.isEmpty() && QFile::exists(fn)) {
        QString sn = cm::execute("cat " + fn);
        qstrcpy(ver->serialNumber, sn.toUtf8().data());
        cfg.app_serialNumber(sn);
    } initRoot(ver->serialNumber);

    int t = QRandomGenerator::global()->bounded(965);
    QTimer::singleShot(t+5,this, &App_Core::initUuid);
}

