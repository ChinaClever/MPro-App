#ifndef DTLS_SENDER_H
#define DTLS_SENDER_H

#include "dtls_recver.h"

class Dtls_Sender : public QObject
{
    Q_OBJECT
public:
    explicit Dtls_Sender(QObject *parent = nullptr);
    bool sendFile(const QStringList &ips, const QString &fn, const sFileTrans &it);

signals:
    void progress(int);
    void errorMessage(const QString &);
    void subProgress(const QString &,int);
    void infoMessage(bool,const QString &);
    void finishSig(const QString &, bool);

public slots:
    void run();
    void throwMessage(const QString &message);

private:
    bool workDown(const QString &host);
    bool writeData(Dtls_Association *dtls);
    void startNewConnection(const QString &address);
    bool send(const QStringList &ips, const QByteArray &array);

private:
    CThread *mThread;
    QByteArray mHead;
    QByteArray mArray;
    QStringList mHosts;
    Dtls_Association *mDtls;
};

#endif // DTLS_SENDER_H
