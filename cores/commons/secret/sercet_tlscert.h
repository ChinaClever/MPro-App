#ifndef SERCET_TLSCERT_H
#define SERCET_TLSCERT_H
#include <QtCore>
#include <QSslKey>
//#include <QtCrypto>
#include <QSslCertificate>
#include <QSslConfiguration>

class Sercret_TlsCert
{
    Sercret_TlsCert();
public:
    static Sercret_TlsCert *bulid();

    QSslKey privKey();
    bool isSelfSigned();
    QSslKey publicKey();
    QByteArray serialNumber();
    QByteArray version();
    QString toText();
    QString issuerInfo(int id);
    QString subjectInfo(int id);
    QByteArray digest();

    QString effectiveDate();
    QString expiryDate();
    QSslConfiguration sslConfiguration();

private:
    QSslCertificate::SubjectInfo toSubjectInfo(int id);
private:
    QSslCertificate *mCert;
};

#endif // SERCET_TLSCERT_H
