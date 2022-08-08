#ifndef SERCET_TLSCERT_H
#define SERCET_TLSCERT_H
#include <QtCore>
#include <QSslCertificate>
#include <QSslKey>

class Sercret_TlsCert
{
public:
    Sercret_TlsCert(const QString &fn, QSsl::EncodingFormat format = QSsl::Pem);

    bool isSelfSigned();
    QSslKey publicKey();
    QByteArray serialNumber();
    QByteArray version();
    QString toText();
    QStringList issuerInfo(QSslCertificate::SubjectInfo info);
    QStringList subjectInfo(QSslCertificate::SubjectInfo info);
    QByteArray digest();

private:
    QSslCertificate *mCert;
};

#endif // SERCET_TLSCERT_H
