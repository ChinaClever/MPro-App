#ifndef DTLS_RECVER_H
#define DTLS_RECVER_H

#include "dtls_service.h"
#include "cfg_com.h"
#include "net_udp.h"

class Dtls_Recver : public QObject
{
    Q_OBJECT
    explicit Dtls_Recver(QObject *parent = nullptr);
public:
    static Dtls_Recver *bulid(QObject *parent = nullptr);
    void setPath(const QString &path) {mIt.path=path;}
    void listen(){mDtls->listen();}
    bool waitForFinish();

signals:
    void startSig(const QString &host);
    void messageSig(const QString &message);
    void finishSig(const sOtaFile &it, bool);

private:
    bool setFile(const QString &fn);
    bool initFile(const QByteArray &data);
    bool recvFinish();

public slots:
    void throwMessage(const QString &message);

private slots:
    void onTimeoutDone();
    void throwError(const QString &message);
    void rcvClientMessage(const QByteArray &data);

private:
    int mSize, mCnt;
    sOtaFile mIt;
    QFile *mFile;
    bool isFinshed;
    Net_Udp *mNet;
    QTimer *mTimer;
    Dtls_Service *mDtls;
};

#endif // DTLS_RECVER_H
