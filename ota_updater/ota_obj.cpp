/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_obj.h"

Ota_Obj::Ota_Obj(QObject *parent)
    : QObject{parent}
{
    system("mkdir -p /tmp/updater/ota_apps");
    QTimer::singleShot(1155,this,SLOT(runing_initFunSlot()));
}

void Ota_Obj::onStartSsdp()
{
    static uint cnt=1, sec=0; if(++cnt%5) return ;
    sRunTime *run = &(cm::masterDev()->proc.core);
    if(run->runSec > sec) {
        sec = run->runSec;
        mSsdp->ssdpClose();
    } else if(!cm::execute("./proc_run cores").toInt()){
        mSsdp->ssdpBind(false);
        //cout << "core stop";
    }
}

void Ota_Obj::runing_initFunSlot()
{
    mTimer = new QTimer(this);
    mTimer->start(1000); mSsdp = new OtaSsdp(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(runing_onTimeoutDone()));
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->proc.ota);
    qstrcpy(run->start, t.toLatin1().data());
    cm::buildDateTime(run->compileTime);
}

void Ota_Obj::runing_onTimeoutDone()
{
    sRunTime *run = &(cm::masterDev()->proc.ota);
    run->runSec += 1; onStartSsdp(); web_workDown();
}



void Ota_Obj::sign_init()
{
    system("echo '' > /tmp/signature.sig");
    system("echo '' > /tmp/public.pem");
    QString fmd = "echo '%1' > /tmp/public.pem";
    QString pubkey = "-----BEGIN PUBLIC KEY-----\n"
            "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuvQJUSeObNKKQYhh5fvB\n"
            "wi6NCnCZ+P679TekB4+o808YOHOFpQvVDc9glzqDmUMIkG7bzL+/43A6zY7ja+gn\n"
            "Mk5BhDOEKNyDHbDRWYIuldkIC2tNM5k5ifrTwfkUyhxvORDVSt03ThU/WME0jJ2u\n"
            "JKS2PYDYghvnULGY01KVEdmnyNdZrUrYedPYv3P90q0GX6ZuQ9EF1VcsZfz3CA/5\n"
            "7E6k6PHNteOJnjtBvzrJIhJHQC9L5UxWE0lpdHx+UvoDmmAQnNLWvlr4etojpCe7\n"
            "X7wFozJ7owKbRcj3Lvkb9mSUe91fPJ1ymWLKWljIo6hXe61YnHcz4qgwRkHaV1gE\n"
            "bQIDAQAB\n"
            "-----END PUBLIC KEY-----";
    system(fmd.arg(pubkey).toLocal8Bit());
}


bool Ota_Obj::sign_verify(const sOtaFile &it)
{
    bool ret = false; sign_init();
    QString pubKey = "/tmp/public.pem";
    QString fmd = "echo '%1' > /tmp/signature.sig";
    QString cmd = fmd.arg(it.sig); system(cmd.toLocal8Bit());
    system("openssl base64 -A -d -in /tmp/signature.sig -out /tmp/sign.sig");
    fmd = "openssl dgst -sha256 -verify %1 -signature /tmp/sign.sig %2";
    cmd = fmd.arg(pubKey, it.path + it.file); QString str = cm::execute(cmd);
    if(str.contains("Verified OK")) ret = true; else qDebug() << str;
    return ret;
}
