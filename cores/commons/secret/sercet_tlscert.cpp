/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercet_tlscert.h"
#include "file.h"

Sercret_TlsCert::Sercret_TlsCert()
{
    QFile crtFile(File::certFile());
    if(crtFile.open(QIODevice::ReadOnly)) {
         mCert = new QSslCertificate(&crtFile, QSsl::Pem);
    } crtFile.close();
}


Sercret_TlsCert *Sercret_TlsCert::bulid()
{
    static Sercret_TlsCert* sington = nullptr;
    if(sington == nullptr) {
        sington = new Sercret_TlsCert();
    }
    return sington;
}

QSslKey Sercret_TlsCert::privKey()
{
    QFile keyFile(File::keyFile());
    if(keyFile.open(QIODevice::ReadOnly)) {
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        keyFile.close();        
        return sslKey;
    }
    return QSslKey();
}

// 返回此证书的加密摘要。
QByteArray Sercret_TlsCert::digest()
{
     QByteArray res;
    if(!mCert->isNull())
        res = mCert->digest();
    return res;
}

// 证书是否是自签名的。颁发者和主题相同则证书被认为是自签名的。
bool Sercret_TlsCert::isSelfSigned()
{
    bool res=false;
    if(!mCert->isNull())
        res = mCert->isBlacklisted();
    return res;
}

// 返回证书主题的公钥。
QSslKey Sercret_TlsCert::publicKey()
{
    QSslKey res;
    if(!mCert->isNull())
        res = mCert->publicKey();
    return res;
}

// 以十六进制格式返回证书的序列号字符串。
QByteArray Sercret_TlsCert::serialNumber()
{
    QByteArray res;
    if(!mCert->isNull())
        res = mCert->serialNumber();
    return res;
}

QByteArray Sercret_TlsCert::version()
{
    QByteArray res;
    if(!mCert->isNull())
        res = mCert->version();
    return res;
}

QString Sercret_TlsCert::toText()
{
    QString res;
    if(!mCert->isNull())
        res = mCert->toText();
    return res;
}

QStringList Sercret_TlsCert::subjectInfo(QSslCertificate::SubjectInfo info)
{
    QStringList res;
    if(!mCert->isNull())
        res = mCert->subjectInfo(info);
    return res;
}

QStringList Sercret_TlsCert::issuerInfo(QSslCertificate::SubjectInfo info)
{
    QStringList res;
    if(!mCert->isNull())
        res = mCert->issuerInfo(info);
    return res;
}
