#ifndef INTEGR_PUSHTHREAD_H
#define INTEGR_PUSHTHREAD_H

#include "net_tcpserver.h"
#include "integr_jsonrecv.h"

struct sPushIt {
    sPushIt() {type=0;sec=5;timeout=1;}
    uchar type; // 1 UDP 2 http
    QString ip; // UDP推送地址
    int port; // UDP推送端口
    QString url; // http网址
    int timeout; // http趕时
    int sec; // 推送时间间隔
};

class Integr_PushThread : public QThread
{
    Q_OBJECT
    explicit Integr_PushThread(QObject *parent = nullptr);
public:
    static Integr_PushThread *bulid(QObject *parent = nullptr);
    ~Integr_PushThread();
    void startFun(const sPushIt &it);
    void stopFun();

private:
    void run();
    void delay();
    void workDown();
    void udpPush(const QByteArray &array);
    void httpPost(const QByteArray &array);

private:
    sPushIt mItem;
    bool isRun = false;
    Net_Udp *mUdp = nullptr;
    Integr_JsonBuild *mJson;
};

#endif // INTEGR_PUSHTHREAD_H
