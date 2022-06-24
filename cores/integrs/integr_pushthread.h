#ifndef INTEGR_PUSHTHREAD_H
#define INTEGR_PUSHTHREAD_H

#include "net_tcpserver.h"
#include "integr_jsonrecv.h"

class Integr_PushThread : public QThread
{
    Q_OBJECT
    explicit Integr_PushThread(QObject *parent = nullptr);
public:
    static Integr_PushThread *bulid(QObject *parent = nullptr);
    void setAddress(const QString &addr) {mHost = addr;}
    void setPort(int port) {mPort = port;}
    ~Integr_PushThread();
    void startFun();
    void stopFun();

protected:
    void run();
    void delay();
    void workDown();

private:
    int mSec = 5;
    bool isRun = false;

    int mPort;
    QString mHost;
    Net_Udp *mUdp = nullptr;
    Integr_JsonBuild *mJson;
};

#endif // INTEGR_PUSHTHREAD_H
