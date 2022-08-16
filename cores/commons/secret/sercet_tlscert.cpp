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
    if(crtFile.exists() && crtFile.open(QIODevice::ReadOnly)) {
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
    if(keyFile.exists() && keyFile.open(QIODevice::ReadOnly)) {
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        keyFile.close();
        return sslKey;
    }
    return QSslKey();
}


QSslConfiguration Sercret_TlsCert::sslConfiguration()
{
    QSslConfiguration ssl;
    QFile certFile(File::certFile());
    QFile keyFile(File::keyFile());

    bool ret = keyFile.open(QIODevice::ReadOnly);
    if(ret) ret = certFile.open(QIODevice::ReadOnly);
    if(ret) {
        QSslCertificate certificate(&certFile, QSsl::Pem);
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        ssl.setLocalCertificate(certificate);
        ssl.setPrivateKey(sslKey);
    }
    return ssl;
}

QString Sercret_TlsCert::effectiveDate()
{
    QString res;
    if(!mCert->isNull())
        res = mCert->effectiveDate().toString("yyyy-MM-dd hh:mm:ss");
    return res;
}

QString Sercret_TlsCert::expiryDate()
{
    QString res;
    if(!mCert->isNull())
        res = mCert->expiryDate().toString("yyyy-MM-dd hh:mm:ss");
    return res;
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
        res = mCert->serialNumber().toHex();
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

QSslCertificate::SubjectInfo Sercret_TlsCert::toSubjectInfo(int id)
{
    QSslCertificate::SubjectInfo info;
    switch (id) {
    case 1: info = QSslCertificate::SubjectInfo::CountryName; break;
    case 2: info = QSslCertificate::SubjectInfo::StateOrProvinceName; break;
    case 3: info = QSslCertificate::SubjectInfo::LocalityName; break;
    case 4: info = QSslCertificate::SubjectInfo::Organization; break;
    case 5: info = QSslCertificate::SubjectInfo::OrganizationalUnitName; break;
    case 6: info = QSslCertificate::SubjectInfo::CommonName; break;
    case 7: info = QSslCertificate::SubjectInfo::EmailAddress; break;
    case 8: info = QSslCertificate::SubjectInfo::SerialNumber; break;
    case 9: info = QSslCertificate::SubjectInfo::DistinguishedNameQualifier; break;
    default: info =  QSslCertificate::SubjectInfo::Organization; break;
    }

    return info;
}

QString Sercret_TlsCert::subjectInfo(int id)
{
    QString res;
    if(!mCert->isNull()) {
        QSslCertificate::SubjectInfo info;
        info = toSubjectInfo(id);
        QStringList ls = mCert->subjectInfo(info);
        if(ls.size()) res = ls.first();
    }
    return res;
}

QString Sercret_TlsCert::issuerInfo(int id)
{
    QString res;
    if(!mCert->isNull()) {
        QSslCertificate::SubjectInfo info;
        info = toSubjectInfo(id);
        QStringList ls = mCert->issuerInfo(info);
        if(ls.size()) res = ls.first();
    }
    return res;
}
