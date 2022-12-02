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
    sOtaUpdater *ota = mOta; setbit(ota->work, DOta_Net);
    qstrcpy(ota->host, host.toUtf8().data());
    ota->net.isRun = 1;
}

QString Ota_Net::unzip(const QString &fn)
{
    QString dst = "/usr/data/"; system("mkdir -p /usr/data/updater/clever/");
    QString str = "unzip -o %1 -d " + dst+"updater/clever/";
    throwMessage(str.arg(fn)); str = cm::execute(str.arg(fn));
    throwMessage(str); //system("rm -rf /usr/data/upload/*");
    return dst+"updater/clever/";
}

bool Ota_Net::coreRuning()
{
    QString cmd = "proc_run cores";
    bool ret = cm::execute(cmd).toInt();
    if(!ret) rebootSlot();
    return ret;
}

int Ota_Net::cmd_updater(const QString &fn, int bit)
{
    QString cmd = "pdu_cmd";
    cmd += " pduCfgSet 83 %1 " + fn;
    int ret = cm::execute(cmd.arg(10+bit)).toInt();
    cmd += " res:" + QString::number(ret);
    throwMessage(cmd.arg(10+bit));
    return ret;
}

bool Ota_Net::up_rootfs(const QString &path)
{
    bool ret = true;
    QString dir = path + "rootfs";
    QStringList fns = File::entryList(dir); cout << dir << fns;
    if(fns.contains("rootfs.squashfs") && fns.contains("xImage")) {
        QString fmd = "system-update %1/xImage %1/rootfs.squashfs";
        sOtaUpdater *ota = &cm::dataPacket()->ota;
        setbit(ota->work, DOta_Rootfs);
        ota->rootfs.progress = 0;
        ota->rootfs.isRun = 1;

        QString str = fmd.arg(dir);
        system(str.toLocal8Bit().data());
        throwMessage(str); cm::mdelay(100);
        ota->rootfs.progress = 100;
        ota->rootfs.isRun = 0;
        cout << ota->rootfs.progress;
        clrbit(cm::dataPacket()->ota.work, DOta_Rootfs);
    } else ret = false;

    return ret;
}

void Ota_Net::workDown(const QString &fn, int bit)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    QString dir = "/usr/data/updater/clever/";
    QString fmd = "rsync -av --exclude rootfs/ %1 /usr/data/clever/";
    if(DOtaCode::DOta_Usb == bit) dir = fn;

    QString cmd = fmd.arg(dir);
    throwMessage(cmd);
    cmd = cm::execute(cmd);
    throwMessage(cmd);
    up_rootfs(dir);

    clrbit(mOta->work, bit);
    bool ret = coreRuning();
    if(ret) cmd_updater(fn, bit);
    if(!mOta->work) QTimer::singleShot(3555,this,SLOT(rebootSlot()));
#endif
}

void Ota_Net::ota_updater(const sOtaFile &it, int bit, bool ok)
{
    setbit(mOta->work, bit);
    sOtaUpIt *up = nullptr; switch (bit) {
    case DOtaCode::DOta_Usb: up = &mOta->usb; break;
    case DOtaCode::DOta_Net: up = &mOta->net; break;
    case DOtaCode::DOta_Web: up = &mOta->web; break;
    default: up = &mOta->net; break;
    } up->isRun = 1;

    QString dir; if(ok) {
        if(it.fc == 21) dir = it.path; // 21时为U盘升级
        else dir = unzip(it.path+it.file);
        ok = versionCheck(dir);
    }

    if(ok) {
        if(QFile::exists(dir+"auto.sh")) {
            QString str = "sh %1/auto.sh ";
            str = cm::execute(str.arg(dir));
            throwMessage(str);
        } else workDown(it.path+it.file, bit);
        up->isRun = 0;
    } else {
        up->isRun = 2;
        if(bit != DOtaCode::DOta_Usb) {
            QString fn = it.path + it.file;
            QString cmd = "rm -f " + fn;
            system(cmd.toLocal8Bit());
            cmd_updater(fn, 400); cm::mdelay(10);
            //system("sync"); system("reboot");
        }
    } clrbit(mOta->work, bit);
}

void Ota_Net::rebootSlot()
{
    system("rm -rf /usr/data/upload");
    system("chmod +x /usr/data/clever/bin/*");
    system("chmod +x /usr/data/clever/app/*");
    system("rm -rf /usr/data/updater/clever");
    system("rm -rf /usr/data/clever/outlet/*");
    throwMessage("start now reboot"); cm::mdelay(1);
    system("sync"); system("reboot");
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
                    } else if(it.md5 == cm::masterDev()->cfg.vers.md5) {
                        QString msg = "version dev md5 err: md5=%1";
                        throwMessage(msg.arg(it.md5)); ret = false;
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
    if(ok && it.sig.size()) {
        ok = sign_verify(it); QString msg = "RSA Verified ";
        if(ok) msg += "ok"; else msg += "error";
        throwMessage(msg);
    } ota_updater(it, DOta_Net, ok);
}
