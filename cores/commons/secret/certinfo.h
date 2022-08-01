#ifndef CERTINFO_H
#define CERTINFO_H
#include <QtCore>
#include <QSslCertificate>
#include <QSslKey>

class CertInfo
{
public:
    CertInfo(const QString &fn, QSsl::EncodingFormat format = QSsl::Pem);

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

#endif // CERTINFO_H
