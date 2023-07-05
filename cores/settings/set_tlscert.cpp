/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_tlscert.h"
#include "sercret_core.h"
#include "set_core.h"

Set_TlsCert::Set_TlsCert()
{

}


QVariant Set_TlsCert::getTlsCert(uchar fc)
{
    QVariant res;
    Sercret_TlsCert *it = Sercret_TlsCert::bulid();
    if(fc > 20) res = it->subjectInfo(fc-20);
    else if(fc > 10) res = it->issuerInfo(fc-10);
    else {
        switch (fc) {
        case 1: res = QString(it->serialNumber());break;
        case 2: res = it->publicKey().length(); break;
        case 3: res = it->effectiveDate(); break;
        case 4: res = it->expiryDate(); break;
        default: qDebug() << Q_FUNC_INFO << fc; break;
        }
    }
    return res;
}

static void tls_log()
{
    cm::mdelay(1540);
    Set_Core::bulid()->upTlsCertLog();
}


bool Set_TlsCert::setTlsCert(uchar fc, const QVariant &v)
{
    //cout << fc << v;
    QString str = v.toString();
    bool ret = true; switch(fc) {
        case 11: mTlsCert.country = str;break;
        case 12: mTlsCert.province = str;break;
        case 13: mTlsCert.city = str;break;
        case 14: mTlsCert.company = str;break;
        case 15: mTlsCert.group = str;break;
        case 16: mTlsCert.certname = str;break;
        case 17: mTlsCert.mail = str;break;
        case 31: ret = createTlsCert();break;
        case 32: if(!mThread) { mThread = new std::thread(tls_log); mThread->detach(); }break;
        //case 32: QTimer::singleShot(1750,this, &Set_TlsCert::upTlsCertLog); break;
        default : ret = false; cout << fc << v; break;
    }
    return ret;
}

bool Set_TlsCert::systemCmd(QString str)
{
    int status = system(str.toLatin1().data());
    if( -1 == status) return false;
    if( ! WIFEXITED(status) ) return false;
    if( WEXITSTATUS(status) ) return false;
    return true;
}

bool Set_TlsCert::createTlsCert()
{
    if(mTlsCert.country.isEmpty() || mTlsCert.province.isEmpty() || mTlsCert.city.isEmpty())
        return false;
    qDebug()<<mTlsCert.country<<"     "<<mTlsCert.province<<"     "<<mTlsCert.city;
    QString sub = QString("/C=%1/ST=%2/L=%3").arg(mTlsCert.country,mTlsCert.province,mTlsCert.city);
    qDebug()<<sub;
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    QString str = "openssl genrsa -out cert/client-key.pem 2048\n";
    bool ret = systemCmd(str);
    str = QString("openssl req -new -out cert/client-req.csr -key cert/client-key.pem -subj %1\n").arg(sub);
    if(ret) ret = systemCmd(str);
    str = "openssl genrsa -out cert/root-key.key 2048\n";
    if(ret) ret = systemCmd(str);
    str = QString("openssl req -new -out cert/root-req.csr -key cert/root-key.key -subj %1\n").arg(sub);
    if(ret) ret = systemCmd(str);
    str = "openssl x509 -req -extfile /usr/lib/ssl/openssl.cnf -extensions v3_req -in cert/client-req.csr -out cert/client-cert.pem -signkey cert/client-key.pem -CAkey cert/root-key.key -CAcreateserial -days 3650";
    if(ret) ret = systemCmd(str);
#else
    QString str = "openssl genrsa -out /usr/data/clever/certs/client-key.pem 2048\n";
    bool ret = systemCmd(str);
    str = QString("openssl req -new -out /usr/data/clever/certs/client-req.csr -key /usr/data/clever/certs/client-key.pem -subj %1\n").arg(sub);
    if(ret) ret = systemCmd(str);
    str = "openssl genrsa -out /usr/data/clever/certs/root-key.key 2048\n";
    if(ret) ret = systemCmd(str);
    str = QString("openssl req -new -out /usr/data/clever/certs/root-req.csr -key /usr/data/clever/certs/root-key.key -subj %1\n").arg(sub);
    if(ret) ret = systemCmd(str);
    str = "openssl x509 -req -extfile /etc/ssl/openssl.cnf -extensions v3_req -in /usr/data/clever/certs/client-req.csr -out /usr/data/clever/certs/client-cert.pem -signkey /usr/data/clever/certs/client-key.pem -CAkey /usr/data/clever/certs/root-key.key -CAcreateserial -days 3650";
    if(ret) ret = systemCmd(str);
#endif
    tlsCertLog(0);
    return ret;
}

void Set_TlsCert::tlsCertName()
{
    QString dir = "/usr/data/clever/certs";
    QStringList fs = File::entryList(dir);
    foreach (const auto &it, fs) {
        if(it == "." || it == "..") continue;
        QString fmd = "mv %1/%2 %1/%3"; QString dst;
        if(it.contains("key.pem")) {
            dst = "client-key.pem";
        } else if(it.contains("cert.pem")) {
            dst = "client-cert.pem";
        } else continue;
        if(dst.size()) cm::execute(fmd.arg(dir, it, dst));
    }
}

void Set_TlsCert::tlsCertLog(int fc)
{
    sEventItem db; QStringList fs;
    QString dir = "/usr/data/clever/certs";
    db.event_type = QStringLiteral("证书管理");
    if(cm::language()) db.event_type = "certificates";
    switch (fc) {
    case 0:
        db.event_content = QStringLiteral("创建新的TLS证书");
        if(cm::language()) db.event_content = "Create a new TLS certificate";
        break;

    case 1:
        db.event_content = QStringLiteral("用户已上传TLS证书");
        if(cm::language()) db.event_content = "User has uploaded TLS certificate ";
        foreach (const auto &it, File::entryList(dir)) db.event_content += it +";";
        tlsCertName(); mThread = nullptr;
        break;

    default: cout << fc; break;
    } Log_Core::bulid()->append(db);
}
