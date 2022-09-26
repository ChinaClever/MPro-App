#ifndef INTEGR_PUSHTHREAD_H
#define INTEGR_PUSHTHREAD_H
#include "mqtt_client.h"
#include "net_tcpserver.h"
#include "integr_jsonrecv.h"
#define INTEGR_UDP_SIZE  2

struct sPushUdpCfg{
    int en = 0;
    QString host;
    int port=1124;
};

struct sPushHttpCfg{
    int en = 0; // 0 禁此 1 post 2 put
    QString url;
    int timeout=1;

    int enServ =0;
    int port=3066;
};

struct sPushCfg{
    sPushUdpCfg udp[INTEGR_UDP_SIZE];
    sPushHttpCfg http;
    int recvPort=3096;
    int recvEn = 0; // 0 禁此 1 UDP 2 TCP 3
    int sec = 5;
};


class Integr_PushThread : public QObject
{
    Q_OBJECT
public:
    explicit Integr_PushThread(QObject *parent = nullptr);
    ~Integr_PushThread();
    static sPushCfg pushCfg;

public slots:
    void run();
    void startSlot();

private:
    void delay();
    void workDown();
    bool checkPush();
    void udpPush(const QByteArray &array);
    void httpPush(const QByteArray &array);
    void mqttPush(const QByteArray &array);

private:
    bool isRun = false;
    Net_Udp *mUdp = nullptr;
    Integr_JsonBuild *mJson;
    CThread *mThread = nullptr;
    sPushCfg *mCfg = &pushCfg;    
};

#endif // INTEGR_PUSHTHREAD_H
