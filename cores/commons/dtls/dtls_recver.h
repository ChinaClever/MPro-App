#ifndef DTLS_RECVER_H
#define DTLS_RECVER_H

#include "commons.h"
#include "dtls_service.h"
#include "cfg_obj.h"


class Dtls_Recver : public QObject
{
    Q_OBJECT
    explicit Dtls_Recver(QObject *parent = nullptr);
public:
    static Dtls_Recver *bulid(QObject *parent = nullptr);
    void setPath(const QString &path) {mIt.recvPath=path;}
    bool waitForFinish();

signals:
    void messageSig(const QString &message);
    void finishSig(const sFileTrans &it, bool);

private:
    bool setFile(const QString &fn);
    bool initFile(const QByteArray &data);
    bool recvFinish();

private slots:
    void throwError(const QString &message);
    void throwMessage(const QString &message);
    void rcvClientMessage(const QByteArray &data);

private:
    int mSize;
    sFileTrans mIt;
    QFile *mFile;
    bool isFinshed;
    Dtls_Service *mDtls;
};

#endif // DTLS_RECVER_H
