#ifndef INTEGR_PUSHTHREAD_H
#define INTEGR_PUSHTHREAD_H

#include "net_tcpserver.h"
#include "integr_jsonrecv.h"
#define INTEGR_UDP_SIZE  2

class Integr_PushThread : public QObject
{
    Q_OBJECT
public:
    explicit Integr_PushThread(QObject *parent = nullptr);
    ~Integr_PushThread();

    void push_startUdp(int id, const QString &ip, int port);
    void push_startHttp(const QString &url, int timeout);
    void push_setTime(int sec=5);
    void push_stop(int id=0);

public slots:
    void run();
    void startSlot();

private:
    void delay();
    void workDown();    
    void udpPush(const QByteArray &array);
    void httpPost(const QByteArray &array);

private:
    bool isRun = false;
    Net_Udp *mUdp = nullptr;
    Integr_JsonBuild *mJson;
    CThread *mThread = nullptr;

    struct sPushIt {
        sPushIt() {sec=5;http_timeout=1;}
        QString udp_ip[INTEGR_UDP_SIZE]; // UDP推送地址
        int port[INTEGR_UDP_SIZE]; // UDP推送端口

        QString http_url; // http网址
        int http_timeout; // http趕时
        int sec; // 推送时间间隔
    } mItem;
};

#endif // INTEGR_PUSHTHREAD_H
