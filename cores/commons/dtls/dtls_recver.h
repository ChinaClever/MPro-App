#ifndef DTLS_RECVER_H
#define DTLS_RECVER_H

#include "commons.h"
#include "dtls_service.h"

class Dtls_Recver : public QObject
{
    Q_OBJECT
public:
    explicit Dtls_Recver(QObject *parent = nullptr);

    bool setFile(const QString &fn);
    int waitForFinish();

signals:
    void finishSig(int);

private slots:
    void throwError(const QString &message);
    void throwMessage(const QString &message);
    void rcvClientMessage(const QByteArray &data);

private:
    int mSize;
    QFile *mFile;
    bool isFinshed;
    Dtls_Service *mDtls;
};

#endif // DTLS_RECVER_H
